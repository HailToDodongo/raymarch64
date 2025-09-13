/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>
#include "rsp_raymarch_layout.h"
#include "../math/mathFP.h"

#define UCODE_DMEM ((volatile UCode::DMEM*)SP_DMEM)

namespace UCode
{
  struct DMEM
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

  inline void run(uint32_t pc = 0)
  {
    *SP_PC = pc;
    MEMORY_BARRIER();
    *SP_STATUS = SP_WSTATUS_CLEAR_HALT | SP_WSTATUS_CLEAR_BROKE | SP_WSTATUS_SET_INTR_BREAK;
//      | SP_WSTATUS_CLEAR_SIG1 | SP_WSTATUS_CLEAR_SIG2;
  }

  inline void resume()
  {
    *SP_STATUS = SP_WSTATUS_CLEAR_HALT | SP_WSTATUS_CLEAR_BROKE | SP_WSTATUS_SET_INTR_BREAK;
  }

  inline void sync()
  {
    while(!(*SP_STATUS & SP_STATUS_HALTED)){}
  }

  inline void stop()
  {
    *SP_STATUS = SP_WSTATUS_SET_HALT;
  }

  inline void reset(const FP32Vec3& rayPos, float lerpFactor, float initialDist)
  {
    UCODE_DMEM->pos[0] = rayPos.x.val;
    UCODE_DMEM->pos[1] = rayPos.y.val;
    UCODE_DMEM->pos[2] = rayPos.z.val;

    uint32_t lerpA = (lerpFactor * 0xFFFF);
    uint32_t lerpB = ((1.0f-lerpFactor) * 0xFFFF);
    UCODE_DMEM->lerpFactorAB = (lerpB << 16) | (lerpA & 0xFFFF);

    FP32 distFP{initialDist};
    UCODE_DMEM->initDist = distFP.val;

    run(RSP_RAY_CODE_Main);
  }

  inline void setRayDirections(const FP32Vec3& dirA, const FP32Vec3& dirB)
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

  inline FP32 getTotalDist(int idx) {
    FP32 distTotal;
    distTotal.val = idx == 0 ? UCODE_DMEM->totalDistA : UCODE_DMEM->totalDistB;
    return distTotal;
  }
}
