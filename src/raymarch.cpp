/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma GCC push_options
#pragma GCC optimize ("-O3")
#pragma GCC optimize ("-ffast-math")

#include <libdragon.h>
#include "raymarch.h"
#include "main.h"
#include "math/mathFloat.h"
#include "math/mathFP.h"
#include "rdp/rdp.h"
#include "rsp/ucode.h"
#include "text.h"
#include "rsp/rsp_raymarch_layout.h"

extern "C" {
  DEFINE_RSP_UCODE(rsp_raymarch);
}

namespace {
  float lerpFactor = 0.5f;

  float cubeSDF(const fm_vec3_t& p) {
    constexpr float s = 0.5f/4.0f;
    fm_vec3_t d{
      fabsf(p.x + 0.25f) - s,
      fabsf(p.y + 0.125f) - s,
      fabsf(p.z + 0.25f) - s,
    };

    d = {
      std::max(d.x, 0.0f),
      std::max(d.y, 0.0f),
      std::max(d.z, 0.0f)
    };

    float distSq = Math::dot(d, d);
    return std::sqrt(distSq);
  }

float cylinderSDF(const fm_vec3_t& p) {
    float r = 0.2f + (p.y*p.y);
    constexpr float h = 0.25f;
    float d[2]{ sqrtf(p.x*p.x + p.z*p.z) - r, fabsf(p.y) - h };
    d[0] = std::max(d[0], 0.0f);
    d[1] = std::max(d[1], 0.0f);
    return std::sqrt(d[0]*d[0] + d[1]*d[1]);
}

fm_vec3_t mainSDFNormals(const fm_vec3_t& p_)
{
    /*assert(p.x > -0.5f);
    assert(p.y > -0.5f);
    assert(p.z > -0.5f);

    assert(p.x < 0.5f);
    assert(p.y < 0.5f);
    assert(p.z < 0.5f);*/

    auto p = Math::fastClamp(p_);
    constexpr float r1 = 0.25f;
    float distXZ = Math::sqrtfApprox(p.x*p.x + p.z*p.z);
    float qx = 1.0f - (r1 / distXZ);
    fm_vec3_t normTorus{
      p.x * qx,
      p.y,
      p.z * qx,
    };
    return Math::normalizeUnsafe(Math::mix(normTorus, p, lerpFactor));
}

float mainSDF(const fm_vec3_t& p) {
    constexpr float r1 = 0.25f;
    constexpr float r2 = 0.075f;

    fm_vec3_t pSq{p.x*p.x, p.y*p.y, p.z*p.z};
    float pSqXZ = pSq.x + pSq.z;

    float distSphere = sqrtf(pSqXZ + pSq.y) - r1;
    float qx = sqrtf(pSqXZ) - r1;
    float distTorus = sqrtf(qx*qx + pSq.y) - r2;

    return Math::mix(distTorus, distSphere, lerpFactor);
}

  constexpr fm_vec3_t RAINBOW_COLORS[4]{
    {31.0f, 10.0f, 10.0f},
    {10.0f, 31.0f, 10.0f},
    {31.0f, 31.0f, 10.0f},
    {31.0f, 31.0f, 31.0f},
  };

  uint32_t shadeResult(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
  {
    float distNorm = (RENDER_DIST - dist);
    float distNormInv = distNorm * (1.0f / RENDER_DIST);

    //uint8_t colr = (res.dist / RENDER_DIST) * 255.0f;
    //return color_to_packed16({colr,colr,colr});

    float light = -Math::dot(norm, dir);
    if(light < 0)light = 0;

    int phase = fm_floorf(hitPos.x+0.55f) * fm_floorf(hitPos.z+0.55f);

    fm_vec3_t col;

    //col.x = 1.0f - distNormInv;
    if(phase & 1) {
      col = RAINBOW_COLORS[(phase >> 1) & 0b11];
      light *= light;
    } else {
      light = 1.0f -light;
      light *= light;
      light = 0.2f + (light * (1.0f - 0.2f));

      col = {norm.x, norm.y, 1.0f};
      col = (col * 15.0f) + 15.0f;
    }
    col *= fm_vec3_t{
      (distNormInv * light),
      (distNormInv * light),
      (distNormInv * light * 2), // shift form the alpha bit
    };

    return ((int)(col.x) << 11) |
      (((int)(col.y) << 6) & 0b00000'11111'00000'0) |
      ((int)(col.z))
    ;
  }

}

void RayMarch::init() {
  rsp_load(&rsp_raymarch);
  UCode::sync();
}

void RayMarch::draw(void* fb, float time, bool lowRes)
{
  auto buff = (char*)fb;

  int W = OUTPUT_WIDTH / (lowRes ? 4 : 1);
  int H = OUTPUT_HEIGHT / (lowRes ? 4 : 1);
  float invH = 1.0f / (float)H;

  float angle = (time + 3.5f) * 0.7f;
  lerpFactor = fm_sinf(time*4.0f) * 0.5f + 0.5f;
  //lerpFactor = 0.5f;

  fm_vec3_t camPos, camDir;

  camPos.x = fm_sinf(angle) * 2.5f;
  camPos.y = fm_cosf(angle-1.1f) * 2.5f;
  camPos.z = fm_sinf(angle*0.6f) * 3.5f;
  camDir = Math::normalize(fm_vec3_t{0,0,0} - camPos);

  float initialDist = mainSDF(Math::fastClamp(camPos));
  UCode::reset({FP32{camPos.x}, FP32{camPos.y}, FP32{camPos.z}}, lerpFactor, initialDist);

  constexpr fm_vec3_t worldUp{0,1,0};
  fm_vec3_t right = Math::normalizeUnsafe(Math::cross(camDir, worldUp));
  fm_vec3_t up = Math::cross(right, camDir);
  auto rightStep = right * invH;
  assert(rightStep.y == 0);

  // 1239.65ms

  buff += (OFFSET_Y * FB_STRIDE) + OFFSET_X*2;
  int stride = FB_STRIDE * (lowRes ? 4 : 1);

  float stepX = (-W/2) * invH;
  float stepY = (-H/2) * invH;
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
        dirFp0 = {FP32{dir0.x}, FP32{dir0.y}, FP32{dir0.z}};
        dirFp1 = {FP32{dir1.x}, FP32{dir1.y}, FP32{dir1.z}};
      };

      auto startNextUcode = [&]() {
        UCode::setRayDirections(dirFp0, dirFp1);
        UCode::run(RSP_RAY_CODE_RayMarch);
      };

      advanceDir();
      UCode::stop();

      MEMORY_BARRIER();
      startNextUcode();
      MEMORY_BARRIER();

      rayDirY += (up * invH);
      uint32_t *buffLocal = (uint32_t*)buff;
      const uint32_t *buffLocalEnd = buffLocal + (W/2);

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
          auto norm = mainSDFNormals(hitPos);
          col |= shadeResult(norm, hitPos, oldDir, distTotal);
        };

        col = 0;
        if(distTotalA < FP32{RENDER_DIST})applyShade(distTotalA.toFloat(), dir0);
        col <<= 16;
        if(distTotalB < FP32{RENDER_DIST})applyShade(distTotalB.toFloat(), dir1);

        *buffLocal = col;
        ++buffLocal;

        advanceDir();

      } while(buffLocal != buffLocalEnd);

      buff += stride;
  }
}

#pragma GCC pop_options