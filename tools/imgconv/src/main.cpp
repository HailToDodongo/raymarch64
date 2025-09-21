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
  uint16_t packRGBA5515(uint8_t r, uint8_t g, uint8_t b, bool a) {
    return ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 0) | (a ? (1<<5) : 0);
  }

  uint16_t packRGBA556(uint8_t r, uint8_t g, uint8_t b) {
    return (((int)r >> 3) << 11) | (((int)g >> 3) << 6) | (((int)b >> 2));
  }
}

void processPNG(const string& fileTex, const string& fileOut, bool texOnly)
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


  uint8_t *dataCol = image.data();
  uint8_t *dataNorm = image.data() + idxNorm;

  if(texOnly) 
  {
    for(unsigned y = 0; y < height; ++y) 
    {
      for(unsigned x = 0; x < width; ++x) 
      {
        writeU16(packRGBA556(dataCol[0],dataCol[1],dataCol[2]));
        dataCol += 4;
      }
    }
  } else {
    assert(width == TEX_DIM);
    assert(height == TEX_DIM*2);

    for(unsigned y = 0; y < TEX_DIM; ++y) 
    {
      for(unsigned x = 0; x < TEX_DIM; ++x) 
      {
        writeS8(dataNorm[0] - 127);
        writeS8(dataNorm[1] - 127);
        
        bool isDefNorm = dataNorm[2] >= 252;
        writeU16(packRGBA5515(dataCol[0],dataCol[1],dataCol[2], isDefNorm));

        dataCol += 4;
        dataNorm += 4;
      }
    }
  }

  fclose(pFile);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <texture.png> <output> [c,n]\n";
        return 1;
    }
    processPNG(argv[1], argv[2], argv[3][0] == 'c');
    return 0;
}
