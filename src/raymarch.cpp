/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma GCC push_options
#pragma GCC optimize ("-O3")
#pragma GCC optimize ("-ffast-math")

namespace {
  constinit float lerpFactor{0};
  //register float lerpFactor asm ("f20");
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

  struct TexPixel
  {
    int8_t normA;
    int8_t normB;
    uint16_t color;
  };
  constexpr int TEX_DIM = 256;



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
    uint32_t bgColor;
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


  inline uint32_t shadeResultEnv(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
  {
    float distNorm = (renderDist - dist);
    float distNormInv = distNorm * renderDistInv;

    // transform normal to screenspace normal
    float normX = -Math::dot(norm, right) * 0.4f + 0.5f;
    float normY = Math::dot(norm, up) * 0.4f + 0.5f;

    float angle = 1.0f - (Math::dot(norm, dir) * 0.5f + 0.5f);

    // Texturing
    float uv[2] {
      normX * TEX_DIM,
      normY * TEX_DIM,
    };

    int uvPixel[2] = {
      (int)(uv[0]) & (TEX_DIM-1),
      (int)(uv[1]) & (TEX_DIM-1),
    };

    TexPixel* texData = (TexPixel*)(MemMap::TEX3_CACHED);
    const TexPixel& tex = texData[uvPixel[1] * TEX_DIM + uvPixel[0]];

    fm_vec3_t col;
    col.x = (tex.color >> 11);
    col.y = (tex.color >> 6) & 0x1F;
    col.z = (tex.color) & 0x1F;

    constexpr fm_vec3_t fresnelCol{31.0f, 31.0f, 31.0f};
    col = Math::mix(
      fresnelCol, col, angle
    );

    col *= (distNormInv);

    return ((int)(col.x) << 11) |
           ((int)(col.y) << 6) |
           ((int)(col.z) << 1)
    ;
  }


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
        };

        auto startNextUcode = [&]() {
          UCode::setRayDirections(dirFp0, dirFp1);
          UCode::run(CONF.fnUcode);
        };

        advanceDir();

        MEMORY_BARRIER();
        startNextUcode();
        UCode::stop();
        startNextUcode();
        MEMORY_BARRIER();

        rayDirY += (up * invH);
        uint32_t *buffLocal = (uint32_t*)buff;
        const uint32_t *buffLocalEnd = buffLocal + (OUTPUT_WIDTH/2);

        advanceDir();

        do
        {
          UCode::sync();
          MEMORY_BARRIER();
          auto distTotalA = UCode::getTotalDist(0);
          auto distTotalB = UCode::getTotalDist(1);

          // Note: this starts the RSP to prepare the next iterations result.
          // the last iteration does so too never reading it, but getting rid of the if-check saves time
          startNextUcode();
          MEMORY_BARRIER();

          uint32_t col = 0;
          auto applyShade = [&](float distTotal, const fm_vec3_t &oldDir) {
            auto hitPos = camPos + (oldDir * distTotal);
            auto norm = CONF.fnNorm(hitPos);
            col |= CONF.fnShade(norm, hitPos, oldDir, distTotal);
          };

          auto writeLowRes = [&]() {
            col = (col << 16) | col;
            if constexpr (SCALING == 4) {
              for(int i=0; i<4; ++i) {
                buffLocal[FB_STRIDE/4*i] = col;
                buffLocal[FB_STRIDE/4*i + 1] = col;
              }
            } else {
              for(int i=0; i<2; ++i) {
                buffLocal[FB_STRIDE/4*i] = col;
              }
            }
            buffLocal += SCALING / 2;
            col = 0;
          };

          if constexpr (!CONF.bgColor) {
            if(distTotalA < renderDistFP)applyShade(distTotalA.toFloat(), dir0);
          } else {
            if(distTotalA < renderDistFP)
              applyShade(distTotalA.toFloat(), dir0);
            else col = CONF.bgColor;
          }

          if constexpr(SCALING > 1)writeLowRes();

          col <<= 16;
          if constexpr (!CONF.bgColor) {
            if(distTotalB < renderDistFP)applyShade(distTotalB.toFloat(), dir1);
          } else {
            if(distTotalB < renderDistFP)
              applyShade(distTotalB.toFloat(), dir1);
            else col |= CONF.bgColor;
          }

          if constexpr (SCALING > 1) {
            writeLowRes();
          } else {
            *buffLocal = col;
            ++buffLocal;
          }

          advanceDir();

        } while(buffLocal != buffLocalEnd);

        buff += stride;
        UCode::stop();
    }
  }

  template<SDFConf CONF>
  inline void drawGenericRes(void* fb, float time, int resFactor)
  {
    switch (resFactor) {
      default:
      case 1: drawGeneric<CONF, 1>(fb, time); break;
      case 2: drawGeneric<CONF, 2>(fb, time); break;
      case 4: drawGeneric<CONF, 4>(fb, time); break;
    }

  }

  constexpr SDFConf SDF_MAIN = {
    SDF::main,
    SDF::mainNormals,
    shadeResultA,
    RSP_RAY_CODE_RayMarch_Main,
    0
  };

  constexpr SDFConf SDF_SPHERE = {
    SDF::sphere,
    SDF::sphereNormals,
    shadeResultPointLight,
    RSP_RAY_CODE_RayMarch_Sphere,
    0
  };

  constexpr SDFConf SDF_CYLINDER = {
    SDF::cylinder,
    SDF::cylinderNormals,
    shadeResultCylinder,
    RSP_RAY_CODE_RayMarch_Cylinder,
    createBgColor({0xFF,0xAA,0xFF})
  };

  constexpr SDFConf SDF_OCTA = {
    SDF::octa,
    SDF::octaNormals,
    shadeResultFlat,
    RSP_RAY_CODE_RayMarch_Octa,
    createBgColor({0xFF,0x55,0x55})
  };

  constexpr SDFConf SDF_TEX = {
    SDF::cylinder,
    SDF::cylinderNormals,
    shadeResultTex,
    RSP_RAY_CODE_RayMarch_Cylinder,
    0,
  };

  constexpr SDFConf SDF_TEX_SPHERE = {
    SDF::main,
    SDF::mainNormals,
    shadeResultTex,
    RSP_RAY_CODE_RayMarch_Main,
    0,
  };

  constexpr SDFConf SDF_ENVMAP = {
    SDF::main,
    SDF::mainNormals,
    shadeResultEnv,
    RSP_RAY_CODE_RayMarch_Main,
    0//createBgColor({0xAA,0xAA,0xFF})
  };

  void loadTexture(const char* path, uint32_t addr) {
    int size = TEXTURE_BYTES;
    auto f = asset_fopen(path, &size);
    fread((void*)addr, 1, size, f);
    fclose(f);
  }
}

void RayMarch::init() {
  rsp_load(&rsp_raymarch);
  UCode::sync();

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
  setRenderDist(RENDER_DIST);

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
      setRenderDist(8);
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
      setRenderDist(3);
      lerpFactor = fm_sinf(time*4.0f) * 0.5f + 0.5f;
      return drawGenericRes<SDF_ENVMAP>(fb, time, resFactor);
  }
}

#pragma GCC pop_options