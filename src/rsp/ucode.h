/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>
#include "rsp_raymarch_layout.h"
#include "dmemLayout.h"
#include "../math/mathFP.h"

namespace UCode
{
  inline void run(uint32_t pc = 0)
  {
    *SP_PC = pc;
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

  inline void setRenderDist(float dist) {
    SP_DMEM[DMEM_RENDER_DIST/4] = FP32{dist}.val;
  }

  inline void reset(const FP32Vec3& rayPos, float lerpFactor, float initialDist)
  {
    SP_DMEM[DMEM_RAYPOS_X/4] = rayPos.x.val;
    SP_DMEM[DMEM_RAYPOS_Y/4] = rayPos.y.val;
    SP_DMEM[DMEM_RAYPOS_Z/4] = rayPos.z.val;


    uint32_t lerpA = (lerpFactor * 0xFFFF);
    uint32_t lerpB = ((1.0f-lerpFactor) * 0xFFFF);
    SP_DMEM[DMEM_LERP_A/4] = (lerpB << 16) | (lerpA & 0xFFFF);

    FP32 distFP{initialDist};
    SP_DMEM[DMEM_INIT_DIST/4] = distFP.val;

    run(RSP_RAY_CODE_Main);
  }

  inline void setRayDirections(const FP32Vec3& dirA, const FP32Vec3& dirB)
  {
    SP_DMEM[DMEM_RAYDIR_A/4 + 0] = dirA.x.val;
    *((uint16_t*)&SP_DMEM[DMEM_RAYDIR_A/4 + 1]) = dirA.z.val;

    SP_DMEM[DMEM_RAYDIR_B/4 + 0] = dirB.x.val;
    *((uint16_t*)&SP_DMEM[DMEM_RAYDIR_B/4 + 1]) = dirB.z.val;
  }

  inline FP32 getTotalDist(int idx) {
    FP32 distTotal;
    distTotal.val = idx == 0 ? SP_DMEM[DMEM_TOTAL_DIST_A/4] : SP_DMEM[DMEM_TOTAL_DIST_B/4];
    return distTotal;
  }
}
