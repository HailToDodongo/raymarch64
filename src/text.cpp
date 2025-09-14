/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include <libdragon.h>
#include "main.h"
#include "text.h"

namespace {
  constexpr uint32_t FMT_BUFF_SIZE = 128;

  constinit int fbStride = 320*2;
  uint8_t *fbBuffer{nullptr};

  #include "font.h"
}

void Text::setFrameBuffer(const surface_t &fb) {
  fbBuffer = (uint8_t*)fb.buffer;
}

int Text::print(int x, int y, const char *str) {
  x &= ~0b11;
  uint64_t *buffStart = (uint64_t*)&fbBuffer[y * fbStride + x*2];

  while(*str)
  {
    uint8_t charCode = (uint8_t)*str - ' ';
    uint64_t charData = FONT_8x8_DATA[charCode];
    uint64_t *buff = buffStart;
    ++x;

    uint64_t val;
    if(charCode != 0)
    {
      for(int y=0; y<8; ++y) {
        for(int x=0; x<2; ++x) {
          val  = (charData & 0b0001) ? ((uint64_t)0xFFFF << 48) : 0;
          val |= (charData & 0b0010) ? ((uint64_t)0xFFFF << 32) : 0;
          val |= (charData & 0b0100) ? ((uint64_t)0xFFFF << 16) : 0;
          val |= (charData & 0b1000) ? ((uint64_t)0xFFFF <<  0) : 0;

          *buff = val;
          charData >>= 4;
          ++buff;
        }
        buff += (fbStride/8 - 2);
      }
      // draw extra black line below
      buff[0] = 0;
      buff[1] = 0;
    }

    buffStart += 2;
    ++str;
  }
  //uint64_t charData = FONT_8x8_DATA[0];

  return x;
}

int Text::printf(int x, int y, const char *fmt, ...) {
  char buffer[FMT_BUFF_SIZE];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, FMT_BUFF_SIZE, fmt, args);
  va_end(args);
  return print(x, y, buffer);
}

