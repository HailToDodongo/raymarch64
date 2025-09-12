/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma GCC push_options
#pragma GCC optimize ("-O3")
#pragma GCC optimize ("-ffast-math")

#include <cmath>
#include <t3d/t3dmath.h>
#include <libdragon.h>
#include "raymarch.h"
#include "main.h"
#include "rdp/rdp.h"
#include "text.h"
#include "rsp/rsp_raymarch_layout.h"

extern "C" {
  DEFINE_RSP_UCODE(rsp_raymarch);
}

/*
#define DPS_TBIST ((volatile uint32_t*)0xA420'0000)
#define DPS_TEST_MODE  ((volatile uint32_t*)0xA420'0004)
#define DPS_BUFTEST_ADDR ((volatile uint32_t*)0xA420'0008)
#define DPS_BUFTEST_DATA ((volatile uint32_t*)0xA420'000C)
*/
namespace {

  struct FP32
  {
    int32_t val{0};

    constexpr FP32() = default;
    constexpr FP32(float f) {
      val = (int32_t)((float)f * 0x10000);
    }
    constexpr FP32(int32_t v) : val(v) {}

    constexpr float toFloat() const {
      return (float)((float)val * (float)(1.0 / 0x10000));
    }

    constexpr FP32 floor() const {
      return { (int32_t)(val & 0xFFFF'0000) };
    }

    FP32& operator=(float f) {
      val = (int32_t)((float)f * 0x10000);
      return *this;
    }

    FP32 operator>>(int bits) const {
      return { val >> bits };
    }
    FP32 operator<(int bits) const {
      return { val << bits };
    }

    FP32 operator+(const FP32& o) const {
      return {val + o.val};
    }
    FP32 operator-(const FP32& o) const {
      return {val - o.val};
    }
    FP32 operator*(const FP32& o) const {
      int64_t r = (int64_t)val * (int64_t)o.val;
      r = r >> 16;
      return {(int32_t)r};
    }

    void operator+=(const FP32& o) {
      val += o.val;
    }
    void operator-=(const FP32& o) {
      val -= o.val;
    }

    bool operator<(const FP32& o) const {
      return val < o.val;
    }
    bool operator<=(const FP32& o) const {
      return val <= o.val;
    }
    bool operator>(const FP32& o) const {
      return val > o.val;
    }
    bool operator>=(const FP32& o) const {
      return val >= o.val;
    }
  };

  struct FP32Vec3 {
    FP32 x{}, y{}, z{};

    constexpr FP32Vec3() = default;
    constexpr FP32Vec3(FP32 x, FP32 y, FP32 z) : x(x), y(y), z(z) {}

    fm_vec3_t toFmVec3() const {
      return { x.toFloat(), y.toFloat(), z.toFloat() };
    }

    FP32Vec3 floor() const {
      return { x.floor(), y.floor(), z.floor() };
    }

    FP32Vec3 operator+(const FP32Vec3& o) const {
      return { x + o.x, y + o.y, z + o.z };
    }
    FP32Vec3 operator-(const FP32Vec3& o) const {
      return { x - o.x, y - o.y, z - o.z };
    }
    FP32Vec3 operator*(const FP32& s) const {
      return { x * s, y * s, z * s };
    }
    void operator+=(const FP32Vec3& o) {
      x += o.x; y += o.y; z += o.z;
    }
    void operator-=(const FP32Vec3& o) {
      x -= o.x; y -= o.y; z -= o.z;
    }
  };

  constexpr uint64_t rgba16To64(uint64_t col) {
    col = (col << 16) | col;
    return (col << 32) | col;
  }

  struct UcodeDMEM
  {
    int32_t pos[3];

    int32_t rayDirA[3];
    int32_t hitPosA[3];
    int32_t lastDistA;
    int32_t totalDistA;

    int32_t rayDirB[3];
    int32_t hitPosB[3];
    int32_t lastDistB;
    int32_t totalDistB;

    int32_t lerpFactorAB;
    int32_t initDist;
  } __attribute__((packed));

  #define UCODE_DMEM ((volatile UcodeDMEM*)SP_DMEM)

  inline void ucode_run(uint32_t pc = 0)
  {
    *SP_PC = pc & 0xFFFF;
    MEMORY_BARRIER();
    *SP_STATUS = SP_WSTATUS_CLEAR_HALT | SP_WSTATUS_CLEAR_BROKE | SP_WSTATUS_SET_INTR_BREAK;
    *SP_STATUS = SP_WSTATUS_CLEAR_SIG0;
  }

  inline void ucode_resume()
  {
    *SP_STATUS = SP_WSTATUS_CLEAR_HALT | SP_WSTATUS_CLEAR_BROKE | SP_WSTATUS_SET_INTR_BREAK;
  }

  inline void ucode_sync()
  {
    while(!(*SP_STATUS & SP_STATUS_HALTED)){}
  }

  inline void ucode_wait_done()
  {
    while(!(*SP_STATUS & SP_STATUS_SIG0)){}
  }

  inline void ucode_reset(const FP32Vec3& rayPos, float lerpFactor, float initialDist)
  {
    UCODE_DMEM->pos[0] = rayPos.x.val;
    UCODE_DMEM->pos[1] = rayPos.y.val;
    UCODE_DMEM->pos[2] = rayPos.z.val;

    uint32_t lerpA = (lerpFactor * 0xFFFF);
    uint32_t lerpB = ((1.0f-lerpFactor) * 0xFFFF);
    UCODE_DMEM->lerpFactorAB = (lerpB << 16) | (lerpA & 0xFFFF);

    FP32 distFP{initialDist};
    UCODE_DMEM->initDist = distFP.val;

    ucode_run(RSP_RAY_CODE_Main);
  }

  inline void ucode_set_ray_dirs(const FP32Vec3& dirA, const FP32Vec3& dirB)
  {
  /*
    constexpr int idx = 128 / 4;
    SP_DMEM[idx+0] = (dirA.x.val & 0xFFFF'0000) | ((uint32_t)dirA.y.val >> 16);
    SP_DMEM[idx+1] = dirA.z.val;
    SP_DMEM[idx+2] = (dirB.x.val & 0xFFFF'0000) | ((uint32_t)dirB.y.val >> 16);
    SP_DMEM[idx+3] = dirB.z.val;

    SP_DMEM[idx+4] = (dirA.x.val << 16) | (dirA.y.val & 0xFFFF);
    SP_DMEM[idx+5] = dirA.z.val << 16;
    SP_DMEM[idx+6] = (dirB.x.val << 16) | (dirB.y.val & 0xFFFF);
    SP_DMEM[idx+7] = dirB.z.val << 16;
    */
    UCODE_DMEM->rayDirA[0] = dirA.x.val;
    UCODE_DMEM->rayDirA[1] = dirA.y.val;
    UCODE_DMEM->rayDirA[2] = dirA.z.val;

    UCODE_DMEM->rayDirB[0] = dirB.x.val;
    UCODE_DMEM->rayDirB[1] = dirB.y.val;
    UCODE_DMEM->rayDirB[2] = dirB.z.val;
  }

  inline FP32Vec3 ucode_get_hit_pos(int idx) {
    return {
      idx == 0 ? UCODE_DMEM->hitPosA[0] : UCODE_DMEM->hitPosB[0],
      idx == 0 ? UCODE_DMEM->hitPosA[1] : UCODE_DMEM->hitPosB[1],
      idx == 0 ? UCODE_DMEM->hitPosA[2] : UCODE_DMEM->hitPosB[2],
    };
  }

  inline FP32 ucode_get_total_dist(int idx) {
    FP32 distTotal;
    distTotal.val = idx == 0 ? UCODE_DMEM->totalDistA : UCODE_DMEM->totalDistB;
    return distTotal;
  }

  inline float sqrtfApprox(float x)
  {
    int32_t i = std::bit_cast<int32_t>(x);
    i  += 127 << 23;
    i >>= 1;
    return std::bit_cast<float>(i);
  }

  inline float dot(const fm_vec3_t& a, const fm_vec3_t& b) {
      return a.x*b.x + a.y*b.y + a.z*b.z;
  }

  inline float length(const fm_vec3_t& v) {
      return std::sqrt(dot(v,v));
  }
  inline fm_vec3_t normalize(const fm_vec3_t& v) {
      float len = length(v);

      if(len < 0.0001f) return v;
      len = 1.0f / len;
      return { v.x*len, v.y*len, v.z*len };
  }

  inline fm_vec3_t normalizeUnsafe(const fm_vec3_t& v) {
      float len = 1.0f / length(v);
      return { v.x*len, v.y*len, v.z*len };
  }

  inline fm_vec3_t cross(const fm_vec3_t& a, const fm_vec3_t& b) {
      return {
          a.y*b.z - a.z*b.y,
          a.z*b.x - a.x*b.z,
          a.x*b.y - a.y*b.x
      };
  }

  using SDFunc = float (*)(const fm_vec3_t&);
  using SDNormFunc = fm_vec3_t (*)(const fm_vec3_t&);
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

    float distSq = dot(d, d);
    return std::sqrt(distSq);
  }

  constexpr float clamp(float v, float mn, float mx) {
    return v < mn ? mn : (v > mx ? mx : v);
  }

  fm_vec3_t fastClamp(const fm_vec3_t& p)
  {
    return {
      (fabsf(p.x) > 0.5f) ? (p.x - fm_floorf(p.x + 0.5f)) : p.x,
      (fabsf(p.y) > 0.5f) ? (p.y - fm_floorf(p.y + 0.5f)) : p.y,
      (fabsf(p.z) > 0.5f) ? (p.z - fm_floorf(p.z + 0.5f)) : p.z,
    };
  }

float mix(float a, float b, float t) {
    //return a * (1.0f - t) + b * t;
    return (b - a) * t + a;
}

fm_vec3_t mix(const fm_vec3_t &a, const fm_vec3_t &b, float t) {
  return {
    mix(a.x, b.x, t),
    mix(a.y, b.y, t),
    mix(a.z, b.z, t),
  };
}

float opSmoothUnion( float d1, float d2 )
{
  float x = 0.5f + 2.5f*(d2-d1);
  float h = clamp(x, 0.0f, 1.0f );
  return mix( d2, d1, h ) - 0.2f*h*(1.0f-h);
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
    float distXZ = sqrtfApprox(p.x*p.x + p.z*p.z);
    float qx = 1.0f - (r1 / distXZ);
    fm_vec3_t normTorus{
      p.x * qx,
      p.y,
      p.z * qx,
    };
    return normalizeUnsafe(mix(normTorus, p, lerpFactor));
}

float mainSDF(const fm_vec3_t& p) {
    constexpr float r1 = 0.25f;
    constexpr float r2 = 0.075f;

    fm_vec3_t pSq{p.x*p.x, p.y*p.y, p.z*p.z};
    float pSqXZ = pSq.x + pSq.z;

    float distSphere = sqrtf(pSqXZ + pSq.y) - r1;
    float qx = sqrtf(pSqXZ) - r1;
    float distTorus = sqrtf(qx*qx + pSq.y) - r2;

    return mix(distTorus, distSphere, lerpFactor);
}

  struct RayRes {
      fm_vec3_t norm{};
      float dist{};
  };

  constexpr float RENDER_DIST = 11.0f;
  constexpr int OUTPUT_WIDTH = 312;
  constexpr int OUTPUT_HEIGHT = 200;
  constexpr int OFFSET_X = 4;
  constexpr int OFFSET_Y = 16;

  static_assert(OUTPUT_WIDTH % 4 == 0); // low-res mode
  static_assert(OUTPUT_HEIGHT % 4 == 0);

  static_assert(OUTPUT_WIDTH % 2 == 0); // 3-pixel step in inner loop...
  static_assert((OUTPUT_WIDTH/4) % 2 == 0); // ...same in low res mode

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

    float light = -dot(norm, dir);
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
  ucode_sync();
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
  camDir = normalize(fm_vec3_t{0,0,0} - camPos);

  float initialDist = mainSDF(fastClamp(camPos));
  ucode_reset({FP32{camPos.x}, FP32{camPos.y}, FP32{camPos.z}}, lerpFactor, initialDist);

  constexpr fm_vec3_t worldUp{0,1,0};
  fm_vec3_t right = normalizeUnsafe(cross(camDir, worldUp));
  fm_vec3_t up = cross(right, camDir);
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

  ucode_sync();

  for(int y=0; y!=H; ++y)
  {
      auto rayDirXY = rayDirY;

      fm_vec3_t dir0, dir1;
      FP32Vec3 dirFp0, dirFp1;

      FP32Vec3 pA, pB;
      FP32 distTotalA, distTotalB;
      bool hasResA, hasResB;
      int iterCount;

      auto advanceDir = [&]() {
        dir0 = normalizeUnsafe(rayDirXY);
        rayDirXY.x += rightStep.x;
        rayDirXY.z += rightStep.z;
        dir1 = normalizeUnsafe(rayDirXY);
        rayDirXY.x += rightStep.x;
        rayDirXY.z += rightStep.z;
        dirFp0 = {FP32{dir0.x}, FP32{dir0.y}, FP32{dir0.z}};
        dirFp1 = {FP32{dir1.x}, FP32{dir1.y}, FP32{dir1.z}};
      };

      auto startNextUcode = [&]() {
        ucode_set_ray_dirs(dirFp0, dirFp1);
        //ucode_set_ray_dir(dirFp1, 1);
        ucode_run(RSP_RAY_CODE_RayMarch);
      };

      auto getUcodeResults = [&]() {
        ucode_sync();

        //iterCount = UCODE_DMEM->lastDistA;

        distTotalA = ucode_get_total_dist(0);
        hasResA = distTotalA < FP32{RENDER_DIST};
        if(hasResA) {
          pA = ucode_get_hit_pos(0);
        }

        distTotalB = ucode_get_total_dist(1);
        hasResB = distTotalB < FP32{RENDER_DIST};
        if(hasResB) {
          pB = ucode_get_hit_pos(1);
        }
      };

      advanceDir();

      MEMORY_BARRIER();
      startNextUcode();
      MEMORY_BARRIER();

      rayDirY += rayDirStepY;
      uint32_t *buffLocal = (uint32_t*)buff;

      for(int x=0; x!=W; x+=2)
      {
        fm_vec3_t oldDir0 = dir0;
        fm_vec3_t oldDir1 = dir1;

        advanceDir();
        getUcodeResults();
        if(x != (W-2)) {
          startNextUcode();
        }
        MEMORY_BARRIER();

        uint32_t col = 0;
        if(hasResA) {
          auto normA = mainSDFNormals(pA.toFmVec3());
          col = shadeResult(normA, camPos, oldDir0, distTotalA.toFloat()) << 16;
        }

        if(hasResB) {
          auto normB = mainSDFNormals(pB.toFmVec3());
          col |= shadeResult(normB, camPos, oldDir1, distTotalB.toFloat()) & 0xFFFF;
        }

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