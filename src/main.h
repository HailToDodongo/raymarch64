/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>

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
}
