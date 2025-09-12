/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once

namespace Text
{
  void setFrameBuffer(const surface_t &fb);
  int print(int x, int y, const char* str);
  int printf(int x, int y, const char *fmt, ...);
}
