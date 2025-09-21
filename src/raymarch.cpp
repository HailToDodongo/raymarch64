/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma GCC push_options
#pragma GCC optimize ("-O3")
#pragma GCC optimize ("-ffast-math")

namespace {
  constinit float lerpFactor{0};
}

#include <libdragon.h>
#include "raymarch.h"
#include "main.h"
#include "math/mathFloat.h"
#include "math/mathFP.h"
#include "rsp/ucode.h"

extern "C" {
  DEFINE_RSP_UCODE(rsp_raymarch);
}

namespace
{
  #include "sdf/sdf.h"

  typedef float (*FuncSDF)(const fm_vec3_t&);
  typedef fm_vec3_t (*FuncNorm)(const fm_vec3_t&);
  typedef uint32_t (*FuncShade)(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist);

  constinit fm_vec3_t lightPos{};
  constinit fm_vec3_t right{};
  constinit fm_vec3_t up{};

  constinit float renderDist = RENDER_DIST;
  constinit float renderDistInv = 1.0f / RENDER_DIST;
  constinit FP32 renderDistFP{RENDER_DIST};

  struct SDFConf
  {
    FuncSDF fnSDF;
    FuncNorm fnNorm;
    FuncShade fnShade;
    uint32_t fnUcode;
    uint32_t bgColor = 0;
    float renderDist;
    bool shadeNoHit = false;
  };

  constexpr uint32_t createBgColor(color_t c) {
    return (((int)c.r >> 3) << 11) | (((int)c.g >> 3) << 6) | (((int)c.b >> 3) << 1) | (c.a >> 7);
  }

  inline void setRenderDist(float dist) {
    UCode::setRenderDist(dist);
    renderDist = dist;
    renderDistInv = 1.0f / dist;
    renderDistFP = FP32{dist};
  }

  #include "shading.h"

  void loadTexture(const char* path, uint32_t addr, int size = TEXTURE_BYTES) {
    auto f = asset_fopen(path, &size);
    fread((void*)addr, 1, size, f);
    fclose(f);
  }

  // We use templates here to intentionally dupe the code.
  // This means things like different SDFs and scaling can be "hardcoded" by the compiler.
  // Since we stay in only one function the entire frame, this saves time since it avoids if-checks.
  template<SDFConf CONF, int SCALING>
  void drawGeneric(void* fb, float time)
  {
    constexpr int W = OUTPUT_WIDTH / SCALING;
    constexpr int H = OUTPUT_HEIGHT / SCALING;
    constexpr float invH = 1.0f / (float)H;

    auto buff = (char*)fb;

    fm_vec3_t camPos = camera.camPos;
    fm_vec3_t camDir = camera.camDir;

    // initial distance is the same for all rays, so do it once here and send it to the ucode
    float initialDist = CONF.fnSDF(Math::fastClamp(camPos));
    // if we start inside an object (negative dist), move out a bit to avoid artifacts
    initialDist = fmaxf(initialDist, 0.11f);
    UCode::reset({FP32{camPos.x}, FP32{camPos.y}, FP32{camPos.z}}, lerpFactor, initialDist);

    constexpr fm_vec3_t worldUp{0,1,0};
    right = Math::normalizeUnsafe(Math::cross(camDir, worldUp));
    up = Math::cross(right, camDir);
    auto rightStep = right * invH;
    assert(rightStep.y == 0);

    buff += (OFFSET_Y * FB_STRIDE) + OFFSET_X*2;
    constexpr int stride = FB_STRIDE * SCALING;

    constexpr float stepX = (-W/2) * invH;
    constexpr float stepY = (-H/2) * invH;
    camDir += right * stepX;

    fm_vec3_t rayDirY = up * stepY + camDir;

    UCode::sync();

    for(int y=0; y!=H; ++y)
    {
        auto rayDirXY = rayDirY;

        fm_vec3_t dir0, dir1;
        FP32Vec3 dirFp0, dirFp1;

        auto advanceDir = [&]() {
          dir0 = Math::normalizeUnsafe(rayDirXY);
          rayDirXY.x += rightStep.x;
          rayDirXY.z += rightStep.z;
          dir1 = Math::normalizeUnsafe(rayDirXY);
          rayDirXY.x += rightStep.x;
          rayDirXY.z += rightStep.z;
          dirFp0 = {FP32::half(dir0.x), FP32::half(dir0.y), FP32::half(dir0.z)};
          dirFp1 = {FP32::half(dir1.x), FP32::half(dir1.y), FP32::half(dir1.z)};

          // pre-shift values for the RSP, which only needs the fractional 16bit part
          dirFp0.x.val = (dirFp0.x.val << 16) | (dirFp0.y.val & 0xFFFF);
          dirFp1.x.val = (dirFp1.x.val << 16) | (dirFp1.y.val & 0xFFFF);
        };

        auto startNextUcode = [&] {
          UCode::setRayDirections(dirFp0, dirFp1);
          UCode::run(CONF.fnUcode);
        };

        advanceDir();

        MEMORY_BARRIER();
        startNextUcode(); // @TODO: not doing this causes glitches on hardware, why?
        UCode::stop();
        startNextUcode();
        MEMORY_BARRIER();

        rayDirY += (up * invH);
        uint16_t *buffLocal = (uint16_t*)buff;
        const uint16_t *buffLocalEnd = buffLocal + OUTPUT_WIDTH;

        advanceDir();

        auto writeColor = [&](uint16_t color)
        {
          constexpr auto xy = [](int x, int y){ return y*FB_STRIDE/2 + x; };
          if constexpr (SCALING == 1) {
            buffLocal[0] = color;
          } else if constexpr (SCALING == 2) {
            buffLocal[xy(0,0)] = color;
            buffLocal[xy(1,0)] = color;
            buffLocal[xy(0,1)] = color;
            buffLocal[xy(1,1)] = color;
          } else if constexpr (SCALING == 4) {
            for (int y=0; y<4; ++y) {
              buffLocal[xy(0,y)] = color;
              buffLocal[xy(1,y)] = color;
              buffLocal[xy(2,y)] = color;
              buffLocal[xy(3,y)] = color;
            }
          }
          buffLocal += SCALING;
        };

        do
        {
          UCode::sync();
          // Note: reading partial results seems to be slower due to the overhead in extra instructions.
          // This can be done by e.g. using the DP_END register as a general purpose reg with 24bits.
          auto distTotalA = UCode::getTotalDist(0);
          auto distTotalB = UCode::getTotalDist(1);

          startNextUcode();
          MEMORY_BARRIER();

          //uint32_t col = 0;
          auto applyShade = [&](float distTotal, const fm_vec3_t &oldDir) {
            if(distTotal >= renderDist) {
              if constexpr (CONF.shadeNoHit) {
                return CONF.fnShade({0,0,0}, {0,0,0}, oldDir, 0);
              }
              return CONF.bgColor;
            }
            auto hitPos = camPos + (oldDir * distTotal);
            auto norm = CONF.fnNorm(hitPos);
            return CONF.fnShade(norm, hitPos, oldDir, distTotal);
          };

          writeColor(applyShade(distTotalA.toFloat(), dir0));
          writeColor(applyShade(distTotalB.toFloat(), dir1));

          advanceDir();

        } while(buffLocal != buffLocalEnd);

        buff += stride;
        UCode::stop();
    }
  }

  template<SDFConf CONF>
  inline void drawGenericRes(void* fb, float time, int resFactor)
  {
    setRenderDist(CONF.renderDist);
    switch (resFactor) {
      default:
      case 1: drawGeneric<CONF, 1>(fb, time); break;
      case 2: drawGeneric<CONF, 2>(fb, time); break;
      case 4: drawGeneric<CONF, 4>(fb, time); break;
    }

  }

  constexpr SDFConf SDF_MAIN = {
    .fnSDF = SDF::main,
    .fnNorm = SDF::mainNormals,
    .fnShade = shadeResultA,
    .fnUcode = RSP_RAY_CODE_RayMarch_Main,
    .renderDist = 11.0f,
  };

  constexpr SDFConf SDF_SPHERE = {
    .fnSDF = SDF::sphere,
    .fnNorm = SDF::sphereNormals,
    .fnShade = shadeResultPointLight,
    .fnUcode = RSP_RAY_CODE_RayMarch_Sphere,
    .renderDist = 11.0f,
  };

  constexpr SDFConf SDF_CYLINDER = {
    .fnSDF = SDF::cylinder,
    .fnNorm = SDF::cylinderNormals,
    .fnShade = shadeResultCylinder,
    .fnUcode = RSP_RAY_CODE_RayMarch_Cylinder,
    .bgColor = createBgColor({0xFF,0xAA,0xFF}),
    .renderDist = 11.0f,
  };

  constexpr SDFConf SDF_OCTA = {
    .fnSDF = SDF::octa,
    .fnNorm = SDF::octaNormals,
    .fnShade = shadeResultFlat,
    .fnUcode = RSP_RAY_CODE_RayMarch_Octa,
    .bgColor = createBgColor({0xFF,0x55,0x55}),
    .renderDist = 11.0f,
  };

  constexpr SDFConf SDF_TEX = {
    .fnSDF = SDF::cylinder,
    .fnNorm = SDF::cylinderNormals,
    .fnShade = shadeResultTex,
    .fnUcode = RSP_RAY_CODE_RayMarch_Cylinder,
    .renderDist = 8.0f,
  };

  constexpr SDFConf SDF_TEX_SPHERE = {
    .fnSDF = SDF::main,
    .fnNorm = SDF::mainNormals,
    .fnShade = shadeResultTex,
    .fnUcode = RSP_RAY_CODE_RayMarch_Main,
    .renderDist = 11.0f,
  };

  constexpr SDFConf SDF_ENVMAP = {
    .fnSDF = SDF::main,
    .fnNorm = SDF::mainNormals,
    .fnShade = shadeResultEnv,
    .fnUcode = RSP_RAY_CODE_RayMarch_Main,
    .bgColor = createBgColor({0xEE,0xEE,0xFF}),
    .renderDist = 5.0f,
  };

  constexpr SDFConf SDF_ENVMAP_2 = {
    .fnSDF = SDF::main,
    .fnNorm = SDF::mainNormals,
    .fnShade = shadeResultEnv2,
    .fnUcode = RSP_RAY_CODE_RayMarch_Main,
    .renderDist = 5.0f,
    .shadeNoHit = true
  };

  constexpr SDFConf SDF_ENVMAP_3 = {
    .fnSDF = SDF::main,
    .fnNorm = SDF::mainNormals,
    .fnShade = shadeResultEnvSky,
    .fnUcode = RSP_RAY_CODE_RayMarch_Main,
    .renderDist = 6.0f,
    .shadeNoHit = true
  };

  constexpr SDFConf SDF_SPHERE_INF = {
    .fnSDF = SDF::sphere,
    .fnNorm = SDF::sphereNormals,
    .fnShade = shadeResultCylinder,
    .fnUcode = RSP_RAY_CODE_RayMarch_Sphere,
    .bgColor = createBgColor({0xFF,0xAA,0xFF}),
    .renderDist = 64.0f,
    .shadeNoHit = false
  };


}

void RayMarch::init() {
  rsp_load(&rsp_raymarch);
  UCode::sync();

  loadTexture("rom:/sky.tex", MemMap::TEX_SKY, 1024*512*2);
  loadTexture("rom:/stone.tex", MemMap::TEX0);
  loadTexture("rom:/tiles.tex", MemMap::TEX1);
  loadTexture("rom:/space.tex", MemMap::TEX2);
  loadTexture("rom:/metal.tex", MemMap::TEX3);

  // generate normal lookup table, this will give the Z component
  // based on a given X and Y component
  for(uint8_t y=0; y<=254; ++y) {
    float fy = (int8_t)y / 127.0f;
    for(uint8_t x=0; x<=254; ++x) {
      float fx = (int8_t)x / 127.0f;
      float fz = 1.0f - fx*fx - fy*fy;
      if(fz <= 0) {
        normalZLookup[y][x] = 0;
      } else {
        fz = sqrtf(fz);
        normalZLookup[y][x] = (int8_t)(fz * 127.0f);
      }
    }
  }

}

void RayMarch::draw(void* fb, float time, int sdfIdx, int resFactor)
{
  switch(sdfIdx)
  {
    case 0:
      lerpFactor = fm_sinf(time*4.0f) * 0.5f + 0.5f;
      return drawGenericRes<SDF_MAIN>(fb, time, resFactor);

    case 1:
      //lerpFactor = fm_sinf(time*1.0f) * 0.125f + 0.125f;
      lerpFactor = 0.275f;
      lightPos = {
        fm_sinf(time*1.7f) * 0.5f,
        fm_cosf(time*1.5f) * 0.5f + 0.5f,
        fm_sinf(time*1.3f + 3.14f) * 0.5f
      };
      //lightPos *= 4.0f;
      return drawGenericRes<SDF_SPHERE>(fb, time, resFactor);

    case 2:
      lerpFactor = fm_sinf(time*3.0f) * 0.1f + 0.15f;
      return drawGenericRes<SDF_CYLINDER>(fb, time, resFactor);

    case 3:
      lerpFactor = (fm_sinf(time*3.0f) * 0.22f + 0.22f) + 0.05f;
      return drawGenericRes<SDF_OCTA>(fb, time, resFactor);

    case 4: {
      lerpFactor = 0.3f;
      lightPos = {
        fm_sinf(time*1.3f),
        0.5f,
        fm_cosf(time*1.3f),
      };
      lightPos = Math::normalize(lightPos);
      return drawGenericRes<SDF_TEX>(fb, time, resFactor);
    }

    case 5:
      //lerpFactor = 0.3f;
      lerpFactor = fm_sinf(time*4.0f) * 0.5f + 0.5f;
      lightPos = {
        fm_sinf(time*1.3f),
        fm_cosf(time*1.5f) * 0.5f + 0.5f,
        fm_cosf(time*1.3f),
      };
      lightPos = Math::normalize(lightPos);
      return drawGenericRes<SDF_TEX_SPHERE>(fb, time, resFactor);

    case 6:
      lerpFactor = fm_sinf(time*4.0f) * 0.5f + 0.5f;
      return drawGenericRes<SDF_ENVMAP>(fb, time, resFactor);

    case 7: 
      lerpFactor = fm_sinf(time*3.0f) * 0.5f + 0.5f;
      return drawGenericRes<SDF_ENVMAP_2>(fb, time, resFactor);

    case 8:
      lerpFactor = fm_sinf(time*2.0f) * 0.5f + 0.5f;
      return drawGenericRes<SDF_ENVMAP_3>(fb, time, resFactor);

    case 9:
      lerpFactor = fm_sinf(time*4.0f) * 0.125f + 0.25f;
      return drawGenericRes<SDF_SPHERE_INF>(fb, time, resFactor);
  }
}

#pragma GCC pop_options
