#include <cstdint>
#include <vector>
#include <array>
#include <cassert>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <iostream>
#include "lodepng.h"

using namespace std;

namespace
{
  uint16_t packRGBA5551(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | (a >> 7);
  }
}

void processPNG(const string& fileTex, const string& fileOut)
{
  vector<unsigned char> image;
  unsigned width, height;

  auto *pFile = fopen(fileOut.c_str(), "wb");

  auto writeU16 = [pFile](uint16_t val) {
    uint8_t high = (val >> 8) & 0xFF;
    uint8_t low = val & 0xFF;
    fwrite(&high, 1, 1, pFile);
    fwrite(&low, 1, 1, pFile);
  };
  auto writeS8 = [pFile](int8_t val) {
    fwrite(&val, 1, 1, pFile);
  };

    unsigned error = lodepng::decode(image, width, height, fileTex);
    if (error) {
        cerr << "PNG loading error: " << lodepng_error_text(error) << endl;
        return;
    }

  constexpr uint32_t TEX_DIM = 256;
  constexpr uint32_t idxNorm = TEX_DIM * TEX_DIM * 4;

  assert(width == TEX_DIM);
  assert(height == TEX_DIM*2);

  uint8_t *dataCol = image.data();
  uint8_t *dataNorm = image.data() + idxNorm;

  for (unsigned y = 0; y < TEX_DIM; ++y) {
    for (unsigned x = 0; x < TEX_DIM; ++x) {
      writeS8(dataNorm[0] - 127);
      writeS8(dataNorm[1] - 127);
      writeU16(packRGBA5551(dataCol[0],dataCol[1],dataCol[2], dataCol[3]));
      dataCol += 4;
      dataNorm += 4;
    }
  }

  fclose(pFile);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <texture.png> <output> \n";
        return 1;
    }
    processPNG(argv[1], argv[2]);
    return 0;
}
