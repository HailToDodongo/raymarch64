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

  inline uint32_t shadeResult(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
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

  template<bool LOW_RES>
  void drawGeneric(void* fb, float time)
  {
    constexpr int W = OUTPUT_WIDTH / (LOW_RES ? 4 : 1);
    constexpr int H = OUTPUT_HEIGHT / (LOW_RES ? 4 : 1);
    constexpr float invH = 1.0f / (float)H;

    auto buff = (char*)fb;

    float angle = (time + 3.5f) * 0.7f;
    lerpFactor = fm_sinf(time*4.0f) * 0.5f + 0.5f;
    //lerpFactor = 0.5f;

    fm_vec3_t camPos, camDir;

    camPos.x = fm_sinf(angle) * 2.55f;
    camPos.y = fm_cosf(angle-1.1f) * 2.45f;
    camPos.z = fm_sinf(angle*0.6f) * 3.15f;
    camDir = Math::normalize(fm_vec3_t{0,0,0} - camPos);

    float initialDist = SDF::main(Math::fastClamp(camPos));
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
          UCode::run(RSP_RAY_CODE_RayMarch);
        };

        advanceDir();

        startNextUcode();
        MEMORY_BARRIER();

        rayDirY += (up * invH);
        uint32_t *buffLocal = (uint32_t*)buff;
        const uint32_t *buffLocalEnd = buffLocal + (OUTPUT_WIDTH/2);

        advanceDir();

        do
        {
          UCode::sync();
          auto distTotalA = UCode::getTotalDist(0);
          auto distTotalB = UCode::getTotalDist(1);

          // Note: this starts the RSP to prepare the next iterations result.
          // the last iteration does so too never reading it, but getting rid of the if-check saves time
          startNextUcode();
          MEMORY_BARRIER();

          uint32_t col;
          auto applyShade = [&](float distTotal, const fm_vec3_t &oldDir) {
            auto hitPos = camPos + (oldDir * distTotal);
            auto norm = SDF::mainNormals(hitPos);
            col |= shadeResult(norm, hitPos, oldDir, distTotal);
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

          col = 0;
          if(distTotalA < FP32{RENDER_DIST})applyShade(distTotalA.toFloat(), dir0);

          if constexpr(LOW_RES)writeLowRes();

          col <<= 16;
          if(distTotalB < FP32{RENDER_DIST})applyShade(distTotalB.toFloat(), dir1);

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
}

void RayMarch::init() {
  rsp_load(&rsp_raymarch);
  UCode::sync();
}

void RayMarch::draw(void* fb, float time, bool lowRes)
{
  if(lowRes) {
    drawGeneric<true>(fb, time);
  } else {
    drawGeneric<false>(fb, time);
  }
}

#pragma GCC pop_options