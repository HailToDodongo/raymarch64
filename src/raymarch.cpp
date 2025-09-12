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

fm_vec3_t mainSDFNormals(const fm_vec3_t& p)
{
    //auto p = fastClamp(p_);
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

  bool lowRes = false;

  constexpr fm_vec3_t RAINBOW_COLORS[4]{
    {31.0f, 10.0f, 10.0f},
    {10.0f, 31.0f, 10.0f},
    {31.0f, 31.0f, 10.0f},
    {31.0f, 31.0f, 31.0f},
  };

  uint32_t shadeResult(const fm_vec3_t &norm, const fm_vec3_t &camPos, const fm_vec3_t &dir, float dist)
  {
    float distNorm = (RENDER_DIST - dist);
    float distNormInv = distNorm * (1.0f / RENDER_DIST);

    //uint8_t colr = (res.dist / RENDER_DIST) * 255.0f;
    //return color_to_packed16({colr,colr,colr});

    float light = -Math::dot(norm, dir);
    if(light < 0)light = 0;

    float hitPosX = camPos.x + (dir.x * dist);
    float hitPosZ = camPos.z + (dir.z * dist);
    int phase = fm_floorf(hitPosX+0.55f) * fm_floorf(hitPosZ+0.55f);

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

void RayMarch::draw(void* fb, float time)
{
  auto press = joypad_get_buttons_pressed(JOYPAD_PORT_1);
  if(press.a)lowRes = !lowRes;

  auto ticks = get_ticks();
  auto buff = (char*)fb;

  int W = OUTPUT_WIDTH / (lowRes ? 4 : 1);
  int H = OUTPUT_HEIGHT / (lowRes ? 4 : 1);
  float invH = 1.0f / (float)H;

  float angle = (time + 3.5f) * 0.7f;
  lerpFactor = fm_sinf(time*4.0f) * 0.5f + 0.5f;
  //lerpFactor = 0.5f;

  fm_vec3_t camPos = {0,0,0};
  fm_vec3_t camDir;

  // orbit camera around 0 based on timer
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

  buff += (OFFSET_Y * FB_STRIDE) + OFFSET_X*2;
  int stride = FB_STRIDE * (lowRes ? 4 : 1);

  float fyStep = invH;
  float stepX = (-W/2) * invH;
  float stepY = (-H/2) * invH;

  fm_vec3_t rayDirStepY = (up * fyStep);
  fm_vec3_t rayDirY = up * stepY + camDir;
  rayDirY += right * stepX;

  UCode::sync();

  for(int y=0; y!=H; ++y)
  {
      auto rayDirXY = rayDirY;

      fm_vec3_t dir0, dir1;
      FP32Vec3 dirFp0, dirFp1;

      FP32Vec3 pA, pB;
      FP32 distTotalA, distTotalB;
      bool hasResA, hasResB;

      auto advanceDir = [&]() {
        dir0 = Math::normalizeUnsafe(rayDirXY);
        dir1 = Math::normalizeUnsafe(rayDirXY + fm_vec3_t{rightStep.x, 0, rightStep.z});
        dirFp0 = {FP32{dir0.x}, FP32{dir0.y}, FP32{dir0.z}};
        dirFp1 = {FP32{dir1.x}, FP32{dir1.y}, FP32{dir1.z}};
      };

      auto startNextUcode = [&]() {
        UCode::setRayDirections(dirFp0, dirFp1);
        UCode::run(RSP_RAY_CODE_RayMarch);
      };

      auto getUcodeResults = [&]() {
        UCode::sync();

        distTotalA = UCode::getTotalDist(0);
        hasResA = distTotalA < FP32{RENDER_DIST};
        if(hasResA) {
          pA = UCode::getHitPos(0);
        }

        distTotalB = UCode::getTotalDist(1);
        hasResB = distTotalB < FP32{RENDER_DIST};
        if(hasResB) {
          pB = UCode::getHitPos(1);
        }
      };

      advanceDir();
      rayDirXY.x += rightStep.x;
      rayDirXY.z += rightStep.z;
      rayDirXY.x += rightStep.x;
      rayDirXY.z += rightStep.z;

      MEMORY_BARRIER();
      startNextUcode();
      MEMORY_BARRIER();

      rayDirY += rayDirStepY;
      uint32_t *buffLocal = (uint32_t*)buff;
      uint32_t col;

      for(int x=0; x!=W; x+=2)
      {
        advanceDir();

        getUcodeResults();
        if(x != (W-2)) {
          startNextUcode();
        }
        MEMORY_BARRIER();

        auto applyShade = [&](const FP32Vec3 &p, const auto &distTotal) {
          auto norm = mainSDFNormals(p.toFmVec3());
          col |= shadeResult(norm, camPos, rayDirXY, distTotal.toFloat());
        };

        col = 0;
        if(hasResA)applyShade(pA, distTotalA);

        rayDirXY.x += rightStep.x;
        rayDirXY.z += rightStep.z;

        col <<= 16;
        if(hasResB)applyShade(pB, distTotalB);

        rayDirXY.x += rightStep.x;
        rayDirXY.z += rightStep.z;

        *buffLocal = col;

        /*if(lowRes) {
          int singleStride = stride/8/4;
          uint64_t *buff64 = (uint64_t*)buffLocal;
          for(int i=2; i>=0; --i) {
            uint64_t col64 = rgba16To64(i == 0 ? (col >> 16) : (col & 0xFFFF));
            buff64[i] = col64;
            buff64[i + singleStride] = col64;
            buff64[i + singleStride*2] = col64;
            buff64[i + singleStride*3] = col64;
          }
          buffLocal += 3;
        }*/

        ++buffLocal;
      }
      buff += stride;
  }

  ticks = get_ticks() - ticks;

  Text::printf(16, 222, "%.2fms``", TICKS_TO_US(ticks) * (1.0f / 1000.0f));
  Text::print(280, 222, lowRes ? "1/4x" : "1x``");
}

#pragma GCC pop_options