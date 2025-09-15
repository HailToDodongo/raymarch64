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
}

void RayMarch::init() {
  rsp_load(&rsp_raymarch);
  UCode::sync();
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
      lightPos *= 4.0f;
      return drawGenericRes<SDF_SPHERE>(fb, time, lowRes);

    case 2:
      lerpFactor = fm_sinf(time*3.0f) * 0.1f + 0.15f;
      return drawGenericRes<SDF_CYLINDER>(fb, time, lowRes);

    case 3:
      lerpFactor = (fm_sinf(time*3.0f) * 0.22f + 0.22f) + 0.05f;
      return drawGenericRes<SDF_OCTA>(fb, time, lowRes);
  }
}

#pragma GCC pop_options