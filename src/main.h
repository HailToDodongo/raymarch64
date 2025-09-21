/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>
#include "camera/flyCam.h"

namespace {
  // global settings
  constexpr uint32_t SCREEN_WIDTH = 320;
  constexpr uint32_t SCREEN_HEIGHT = 240;
  constexpr uint32_t FB_STRIDE = SCREEN_WIDTH * 2;

  // ray-marcher settings
  constexpr float RENDER_DIST = 16.0f;
  constexpr int OUTPUT_WIDTH = 312;
  constexpr int OUTPUT_HEIGHT = 200;
  constexpr int OFFSET_X = 4;
  constexpr int OFFSET_Y = 16;

  static_assert(OUTPUT_WIDTH % 4 == 0); // low-res mode
  static_assert(OUTPUT_HEIGHT % 4 == 0);

  static_assert(OUTPUT_WIDTH % 2 == 0); // 3-pixel step in inner loop...
  static_assert((OUTPUT_WIDTH/4) % 2 == 0); // ...same in low res mode

  constexpr uint32_t TEXTURE_DIM = 256;
  constexpr uint32_t TEXTURE_BYTES = TEXTURE_DIM * TEXTURE_DIM * 4;
}

namespace MemMap
{
  constexpr uint32_t FB0 = 0xA010'0000;
  constexpr uint32_t FB1 = 0xA014'0000;
  constexpr uint32_t FB2 = 0xA018'0000;

  constexpr uint32_t TEX0 = 0xA01C'0000;
  constexpr uint32_t TEX0_CACHED = 0x801C'0000;

  constexpr uint32_t TEX1 = 0xA020'0000;
  constexpr uint32_t TEX1_CACHED = 0x8020'0000;

  constexpr uint32_t TEX2 = 0xA024'0000;
  constexpr uint32_t TEX2_CACHED = 0x8024'0000;

  constexpr uint32_t TEX3 = 0xA028'0000;
  constexpr uint32_t TEX3_CACHED = 0x8028'0000;

  constexpr uint32_t TEX_SKY = 0xA02C'0000;
  constexpr uint32_t TEX_SKY_CACHED = 0x802C'0000;

}

// Globals
extern FlyCam camera;