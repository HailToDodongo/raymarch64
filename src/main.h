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
  constexpr float RENDER_DIST = 11.0f;
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
  constexpr uint32_t FB0 = 0xA024'0000;
  constexpr uint32_t FB1 = 0xA028'0000;
  constexpr uint32_t FB2 = 0xA030'0000;

  constexpr uint32_t TEX0 = 0xA034'0000;
  constexpr uint32_t TEX0_CACHED = 0x8034'0000;

  constexpr uint32_t TEX1 = 0xA038'0000;
  constexpr uint32_t TEX1_CACHED = 0x8038'0000;

  constexpr uint32_t TEX2 = 0xA03C'0000;
  constexpr uint32_t TEX2_CACHED = 0x803C'0000;
}

// Globals
extern FlyCam camera;