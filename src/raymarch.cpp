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

  fm_vec3_t lightPos{0,0,0};

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

  inline uint32_t shadeResultA(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
  {
    float distNorm = (RENDER_DIST - dist);
    float distNormInv = distNorm * (1.0f / RENDER_DIST);

    //uint8_t colr = (res.dist / RENDER_DIST) * 255.0f;
    //return color_to_packed16({colr,colr,colr});

    float light = -Math::dot(norm, dir);
    light = fmaxf(light, 0);

    int phase = (int)(hitPos.x+0.55f) * (int)(hitPos.z+0.55f);

    fm_vec3_t col;

    //col.x = 1.0f - distNormInv;
    if(phase & 1) {
      switch(phase & 0b110) {
        case 0b000: col = {31.0f, 10.0f, 10.0f}; break;
        case 0b010: col = {10.0f, 31.0f, 10.0f}; break;
        case 0b100: col = {31.0f, 31.0f, 10.0f}; break;
        case 0b110: col = {31.0f, 31.0f, 31.0f}; break;
      }
      light *= light;
    } else {
      light = 1.0f -light;
      light *= light;
      light = 0.2f + (light * (1.0f - 0.2f));

      col = fm_vec3_t{norm.x * 15.0f, norm.y * 15.0f, 15.0f} + 15.0f;
    }

    col *= (distNormInv * light);

    return ((int)(col.x) << 11) |
           ((int)(col.y) << 6) |
           ((int)(col.z) << 1)
    ;
  }

  inline uint32_t shadeResultCylinder(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
  {
    float distNorm = (RENDER_DIST - dist);
    float distNormInv = distNorm * (1.0f / RENDER_DIST);

    float light = -Math::dot(norm, dir);
    light = fmaxf(light, 0);

    float s = hitPos.y * 24;
    float base = ((int)(s) & 1) ? 5.0f : 15.0f;
    s = hitPos.y*2;

    fm_vec3_t col{
      Math::sinApprox(s + 0.0f) * base + base,
      Math::sinApprox(s + 2.0f) * base + base,
      Math::sinApprox(s + 4.0f) * base + base,
    };

    //col *= (distNormInv * light);
    col = Math::mix(
      {22.0f, 22.0f, 31.0f}, col * light, distNormInv
    );

    return ((int)(col.x) << 11) |
           ((int)(col.y) << 6) |
           ((int)(col.z) << 1)
    ;
  }

  inline uint32_t shadeResultFlat(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
  {
    float distNorm = (RENDER_DIST - dist);
    float distNormInv = distNorm * (1.0f / RENDER_DIST);

    float light = -Math::dot(norm, dir);
    light = fmaxf(light, 0);
    light = fminf(light + 0.25f, 1);

    int phase = (int)(hitPos.x+0.5f) + (int)(hitPos.z+0.5f) + (int)(hitPos.y+0.5f);
    float s = phase * 32;

    constexpr float base = 15.5f;

    fm_vec3_t col{
      Math::sinApprox(s + 0.0f) * base + base,
      Math::sinApprox(s + 2.0f) * base + base,
      Math::sinApprox(s + 4.0f) * base + base,
    };

    col = Math::mix(
      {31.0f, 11.0f, 11.0f}, col * light, distNormInv
    );

    return ((int)(col.x) << 11) |
           ((int)(col.y) << 6) |
           ((int)(col.z) << 1)
    ;
  }

  inline uint32_t shadeResultPointLight(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
  {
    float distNorm = (RENDER_DIST - dist);
    float distNormInv = distNorm * (1.0f / RENDER_DIST);

    constexpr float LIGHT_RANGE = 1.0f / 5.0f;
    fm_vec3_t toLight = lightPos - hitPos;
    float ptLightDist = Math::length(toLight);
    ptLightDist *= LIGHT_RANGE;
    ptLightDist *= ptLightDist;

    float lightPoint = Math::dot(norm, Math::normalizeUnsafe(toLight));
    lightPoint = fmaxf(lightPoint, 0);
    lightPoint = fminf(lightPoint + 0.0125f, 1);
    lightPoint *= (1.0f - Math::clamp(ptLightDist, 0.0f, 1.0f));

    float light = lightPoint;

    constexpr float base = 15.5f;

    fm_vec3_t col;

    int phase = (int)(hitPos.x+0.5f) + (int)(hitPos.z+0.5f) + (int)(hitPos.y+0.5f);
    switch(phase & 0b110) {
        default:
        case 0b000: col = {31.0f, 15.0f, 15.0f}; break;
        case 0b010: col = {15.0f, 31.0f, 15.0f}; break;
        case 0b100: col = {31.0f, 31.0f, 15.0f}; break;
        case 0b110: col = {31.0f, 31.0f, 31.0f}; break;
      }

    /*col = Math::mix(
      {31.0f, 11.0f, 11.0f}, col * light, distNormInv
    );*/
    col *= (light * distNormInv);

    return ((int)(col.x) << 11) |
           ((int)(col.y) << 6) |
           ((int)(col.z) << 1)
    ;
  }

  inline fm_vec3_t rotVecY(const fm_vec3_t &v, float c, float s)
  {
    return {
      v.x * c - v.z * s,
      v.y,
      v.x * s + v.z * c,
    };
  }

  inline fm_vec3_t rotVecY(const fm_vec3_t &v, float angle)
  {
    return rotVecY(v, fm_cosf(angle), fm_sinf(angle));
  }

  constinit int8_t normalZLookup[256][256]{};

  inline uint32_t shadeResultTex(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
  {
    float distNorm = (RENDER_DIST - dist);
    float distNormInv = distNorm * (1.0f / RENDER_DIST);

    int phase = (int)(hitPos.x+0.5f) + (int)(hitPos.z+0.5f);

    float angleY = fm_atan2f(norm.z, norm.x);// - (3.124f / 2);
    constexpr float ANGLE_TO_UV = (1.0f / (2.0f * 3.124f)) * (TEX_DIM * -2.0f);

    // Texturing
    float uv[2] {
      angleY * ANGLE_TO_UV,
      hitPos.y * (1.2f * TEX_DIM),
    };

    int uvPixel[2] = {
      (int)(uv[0]) & (TEX_DIM-1),
      (int)(uv[1]) & (TEX_DIM-1),
    };

    TexPixel* texData;
    switch (phase & 0b11) {
      default:
      case 0: texData = (TexPixel*)MemMap::TEX0_CACHED; break;
      case 1: texData = (TexPixel*)MemMap::TEX1_CACHED; break;
      case 2: texData = (TexPixel*)MemMap::TEX2_CACHED; break;
    }

    const TexPixel& tex = texData[uvPixel[1] * TEX_DIM + uvPixel[0]];

    fm_vec3_t col;
    col.x = (tex.color >> 11);
    col.y = (tex.color >> 6) & 0x1F;
    col.z = (tex.color) & 0x1F;

    fm_vec3_t normTex = {
      (tex.normA * (1.0f / 128.0f)),
      (tex.normB * (1.0f / 128.0f)),
      0,
    };

    //auto held = joypad_get_buttons_held(JOYPAD_PORT_1);
    normTex.z = sqrtf(1.0f - normTex.x*normTex.x - normTex.z*normTex.z);

    normTex = rotVecY(normTex, norm.x, norm.z);

    float lightPoint = Math::dot(normTex, lightPos);
    lightPoint = fmaxf(lightPoint, 0);

    auto lightColor = fm_vec3_t{1.0f, 0.8f, 0.6f} * lightPoint;
    constexpr auto ambientColor = fm_vec3_t{0.15f, 0.15f, 0.3f};

    lightColor = Math::min(lightColor + ambientColor, {1,1,1});

    col *= lightColor;
    col *= distNormInv;

    return ((int)(col.x) << 11) |
           ((int)(col.y) << 6) |
           ((int)(col.z) << 1)
    ;
  }

  template<SDFConf CONF, bool LOW_RES>
  void drawGeneric(void* fb, float time)
  {
    constexpr int W = OUTPUT_WIDTH / (LOW_RES ? 4 : 1);
    constexpr int H = OUTPUT_HEIGHT / (LOW_RES ? 4 : 1);
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
    fm_vec3_t right = Math::normalizeUnsafe(Math::cross(camDir, worldUp));
    fm_vec3_t up = Math::cross(right, camDir);
    auto rightStep = right * invH;
    assert(rightStep.y == 0);

    buff += (OFFSET_Y * FB_STRIDE) + OFFSET_X*2;
    constexpr int stride = FB_STRIDE * (LOW_RES ? 4 : 1);

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
            for(int i=0; i<4; ++i) {
              buffLocal[FB_STRIDE/4*i] = col;
              buffLocal[FB_STRIDE/4*i + 1] = col;
            }
            buffLocal += 2;
            col = 0;
          };

          if constexpr (!CONF.bgColor) {
            if(distTotalA < FP32{RENDER_DIST})applyShade(distTotalA.toFloat(), dir0);
          } else {
            if(distTotalA < FP32{RENDER_DIST})
              applyShade(distTotalA.toFloat(), dir0);
            else col = CONF.bgColor;
          }

          if constexpr(LOW_RES)writeLowRes();

          col <<= 16;
          if constexpr (!CONF.bgColor) {
            if(distTotalB < FP32{RENDER_DIST})applyShade(distTotalB.toFloat(), dir1);
          } else {
            if(distTotalB < FP32{RENDER_DIST})
              applyShade(distTotalB.toFloat(), dir1);
            else col |= CONF.bgColor;
          }

          if constexpr (LOW_RES) {
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
  inline void drawGenericRes(void* fb, float time, bool lowRes)
  {
    if(lowRes) {
      drawGeneric<CONF, true>(fb, time);
    } else {
      drawGeneric<CONF, false>(fb, time);
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

void RayMarch::draw(void* fb, float time, int sdfIdx, bool lowRes)
{
  switch(sdfIdx)
  {
    case 0:
      lerpFactor = fm_sinf(time*4.0f) * 0.5f + 0.5f;
      return drawGenericRes<SDF_MAIN>(fb, time, lowRes);

    case 1:
      //lerpFactor = fm_sinf(time*1.0f) * 0.125f + 0.125f;
      lerpFactor = 0.275f;
      lightPos = {
        fm_sinf(time*1.7f) * 0.5f,
        fm_cosf(time*1.5f) * 0.5f + 0.5f,
        fm_sinf(time*1.3f + 3.14f) * 0.5f
      };
      //lightPos *= 4.0f;
      return drawGenericRes<SDF_SPHERE>(fb, time, lowRes);

    case 2:
      lerpFactor = fm_sinf(time*3.0f) * 0.1f + 0.15f;
      return drawGenericRes<SDF_CYLINDER>(fb, time, lowRes);

    case 3:
      lerpFactor = (fm_sinf(time*3.0f) * 0.22f + 0.22f) + 0.05f;
      return drawGenericRes<SDF_OCTA>(fb, time, lowRes);

    case 4: {
      lerpFactor = 0.3f;
      lightPos = {
        fm_sinf(time*1.3f),
        0.5f,
        fm_cosf(time*1.3f),
      };
      lightPos = Math::normalize(lightPos);
      return drawGenericRes<SDF_TEX>(fb, time, lowRes);
    }

    case 5:
      //lerpFactor = 0.3f;
      lerpFactor = fm_sinf(time*4.0f) * 0.5f + 0.5f;
      lightPos = {
        fm_sinf(time*1.3f),
        fm_cosf(time*1.5f) * 0.5f + 0.5f,
        fm_cosf(time*1.3f),
      };
      return drawGenericRes<SDF_TEX_SPHERE>(fb, time, lowRes);
  }
}

#pragma GCC pop_options