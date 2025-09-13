/**
* @copyright 2024 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>
#include <bit>
#include <vector>

namespace 
{
  constexpr uint64_t bitVal(uint64_t value, uint32_t startBit, uint32_t endBit) {
    // mask out value and shift into position
    uint64_t mask = (1 << (startBit - endBit + 1)) - 1;
    return (uint64_t)(value & mask) << (endBit);
  }

  constexpr uint64_t bitCmd(uint32_t cmd) {
    return bitVal(cmd, 61, 56);
  }

  constexpr uint64_t floatTo10p2(float value) {
    return (uint32_t)(value * 4);
  }

  constexpr uint64_t floatTo10p5(float value) {
    return (uint32_t)(value * 32);
  }

  constexpr uint64_t floatTo5p10(float value) {
    return (uint32_t)(value * 1024);
  }

  constexpr uint64_t floatTo1p11(float value) {
    return (uint32_t)(value * 2048);
  }

  constexpr uint32_t addrToPhysical(void* addr) {
    return std::bit_cast<uint32_t>(addr) & ~0xE0000000;
  }
}

namespace RDP 
{
  namespace Format {
    constexpr uint32_t RGBA = 0;
    constexpr uint32_t YUV = 1;
    constexpr uint32_t CI = 2;
    constexpr uint32_t IA = 3;
    constexpr uint32_t I = 4;
  }
    
  namespace BBP {
    constexpr uint32_t _4 = 0;
    constexpr uint32_t _8 = 1;
    constexpr uint32_t _16 = 2;
    constexpr uint32_t _32 = 3;
  }

  namespace CYCLE {
    constexpr uint32_t ONE = 0;
    constexpr uint32_t TWO = 1;
    constexpr uint32_t COPY = 2;
    constexpr uint32_t FILL = 3;
  }

  struct CCCycle {
    uint32_t colorA;
    uint32_t colorB;
    uint32_t colorC;
    uint32_t colorD;
    uint32_t alphaA;
    uint32_t alphaB;
    uint32_t alphaC;
    uint32_t alphaD;
  };

  // Enums for CC inputs
  namespace CC {
    namespace C_A {
      constexpr uint32_t COMBINED = 0;
      constexpr uint32_t TEX0 = 1;
      constexpr uint32_t TEX1 = 2;
      constexpr uint32_t PRIM = 3;
      constexpr uint32_t SHADE = 4;
      constexpr uint32_t ENV = 5;
      constexpr uint32_t ONE = 6;
      constexpr uint32_t NOISE = 7;
      constexpr uint32_t ZERO = 8;
    }
    namespace C_B {
      constexpr uint32_t COMBINED = 0;
      constexpr uint32_t TEX0 = 1;
      constexpr uint32_t TEX1 = 2;
      constexpr uint32_t PRIM = 3;
      constexpr uint32_t SHADE = 4;
      constexpr uint32_t ENV = 5;
      constexpr uint32_t CENTER = 6;
      constexpr uint32_t K4 = 7;
      constexpr uint32_t ZERO = 8;
    }
    namespace C_C {
      constexpr uint32_t COMBINED = 0;
      constexpr uint32_t TEX0 = 1;
      constexpr uint32_t TEX1 = 2;
      constexpr uint32_t PRIM = 3;
      constexpr uint32_t SHADE = 4;
      constexpr uint32_t ENV = 5;
      constexpr uint32_t CENTER = 6;
      constexpr uint32_t COMBINED_ALPHA = 7;
      constexpr uint32_t TEX0_ALPHA = 8;
      constexpr uint32_t TEX1_ALPHA = 9;
      constexpr uint32_t PRIM_ALPHA = 10;
      constexpr uint32_t SHADE_ALPHA = 11;
      constexpr uint32_t ENV_ALPHA = 12;
      constexpr uint32_t LOD_FRACTION = 13;
      constexpr uint32_t PRIM_LOD_FRAC = 14;
      constexpr uint32_t K5 = 15;
      constexpr uint32_t ZERO = 16;
    }
    namespace C_D {
      constexpr uint32_t COMBINED = 0;
      constexpr uint32_t TEX0 = 1;
      constexpr uint32_t TEX1 = 2;
      constexpr uint32_t PRIM = 3;
      constexpr uint32_t SHADE = 4;
      constexpr uint32_t ENV = 5;
      constexpr uint32_t ONE = 6;
      constexpr uint32_t ZERO = 7;
    }

    namespace A_ABD {
      constexpr uint32_t COMBINED = 0;
      constexpr uint32_t TEX0 = 1;
      constexpr uint32_t TEX1 = 2;
      constexpr uint32_t PRIM = 3;
      constexpr uint32_t SHADE = 4;
      constexpr uint32_t ENV = 5;
      constexpr uint32_t ONE = 6;
      constexpr uint32_t ZERO = 7;
    }
    namespace A_C {
      constexpr uint32_t LOD_FRAC = 0;
      constexpr uint32_t TEX0 = 1;
      constexpr uint32_t TEX1 = 2;
      constexpr uint32_t PRIM = 3;
      constexpr uint32_t SHADE = 4;
      constexpr uint32_t ENV = 5;
      constexpr uint32_t PRIM_LOD_FRAC = 6;
      constexpr uint32_t ZERO = 7;
    }

    constexpr const char* CC_C_A[] = {"COMB", "TEX0", "TEX1", "PRIM", "SHADE", "ENV", "1", "NOISE", "0"};
    constexpr const char* CC_C_B[] = {"COMB", "TEX0", "TEX1", "PRIM", "SHADE", "ENV", "CENTER", "K4", "0"};
    constexpr const char* CC_C_C[] = {"COMB", "TEX0", "TEX1", "PRIM", "SHADE", "ENV", "CENTER", "COMB_ALPHA", "TEX0_A", "TEX1_A", "PRIM_A", "SHADE_A", "ENV_A", "LOD_FRAC", "PRIM_LOD_FRAC", "K5", "0"};
    constexpr const char* CC_C_D[] = {"COMB", "TEX0", "TEX1", "PRIM", "SHADE", "ENV", "1", "0"};

    constexpr const char* CC_A_ABD[] = {"COMB", "TEX0", "TEX1", "PRIM", "SHADE", "ENV", "1", "0"};
    constexpr const char* CC_A_C[] = {"LOD_FRAC", "TEX0", "TEX1", "PRIM", "SHADE", "ENV", "PRIM_LOD_FRAC", "0"};
  }

  struct Vertex {
    float pos[2]{0.0f, 0.0f};
    float uv[2]{0.0f, 0.0f};
    float color[4]{0, 0, 0, 0};
    float depth{0.0f};
  };

  struct TriParams {
    uint32_t lft;
    int32_t y1f, y2f, y3f;
    float xl, xm, xh;
    float isl, ism, ish;
    uint32_t final_rgba[4];
    float DrgbaDx[4];
    float DrgbaDe[4];
    float DrgbaDy[4];
  };

  namespace TriAttr {
    constexpr uint32_t POS     = 0;
    constexpr uint32_t SHADE   = 1 << 0;
    constexpr uint32_t TEXTURE = 1 << 1;
    constexpr uint32_t DEPTH   = 1 << 2;
  }

  inline void dumpRegisters() {
    debugf("RDP DP_START    : %08lx\n", *DP_START);
    debugf("RDP DP_END      : %08lx\n", *DP_END);
    debugf("RDP DP_CURRENT  : %08lx\n", *DP_CURRENT);
    debugf("RDP DP_STATUS   : %08lx\n", *DP_STATUS);
    debugf("RDP DP_CLOCK    : %08lx\n", *DP_CLOCK);
    debugf("RDP DP_BUSY     : %08lx\n", *DP_BUSY);
    debugf("RDP DP_PIPE_BUSY: %08lx\n", *DP_PIPE_BUSY);
    debugf("RDP DP_TMEM_BUSY: %08lx\n", *DP_TMEM_BUSY);
  }

  constexpr uint64_t setColorImage(void* colorBuff, uint32_t format, uint32_t bbp, uint32_t width) {
    return bitCmd(0x3F)
      | bitVal(format, 55, 53)
      | bitVal(bbp, 52, 51)
      | bitVal(width-1, 41, 32)
      | bitVal(addrToPhysical(colorBuff), 23, 0);
  }

  constexpr uint64_t setConvert(uint8_t k0, uint8_t  k1, uint8_t  k2, uint8_t  k3, uint8_t  k4, uint8_t  k5) {
    return bitCmd(0x2C)
      | bitVal(k0, 53, 45)
      | bitVal(k1, 44, 36)
      | bitVal(k2, 35, 27)
      | bitVal(k3, 26, 18)
      | bitVal(k4, 17, 9)
      | bitVal(k5, 8, 0);
  }

  constexpr uint64_t setKeyGB(
    uint16_t widthG, uint8_t centerG, uint8_t scaleG,
    uint16_t widthB, uint8_t centerB, uint8_t scaleB
  ) {
    return bitCmd(0x2A)
      | bitVal(widthG, 55, 44)
      | bitVal(widthB, 43, 32)
      | bitVal(centerG, 31, 24)
      | bitVal(scaleG, 23, 16)
      | bitVal(centerB, 15, 8)
      | bitVal(scaleB, 7, 0);
  }

  constexpr uint64_t setKeyR(uint16_t widthR, uint8_t centerR, uint8_t scaleR) {
    return bitCmd(0x2B)
      | bitVal(widthR, 27, 16)
      | bitVal(centerR, 15, 8)
      | bitVal(scaleR, 7, 0);
  }

  consteval uint64_t setSyncLoad() { return bitCmd(0x26); }
  consteval uint64_t setSyncPipe() { return bitCmd(0x27); }
  consteval uint64_t setSyncTile() { return bitCmd(0x28); }
  consteval uint64_t setSyncFull() { return bitCmd(0x29); }

  constexpr uint64_t setFillColor(color_t rgba)
  {
    auto packColor = [](color_t c) -> uint16_t {
      return (((int)c.r >> 3) << 11) | (((int)c.g >> 3) << 6) | (((int)c.b >> 3) << 1) | (c.a >> 7);
    };

    return bitCmd(0x37)
      | ((uint64_t)packColor(rgba) << 16)
      | ((uint64_t)packColor(rgba) <<  0);
  }

  constexpr uint64_t setFillColorRaw(uint32_t value)
  {
    return bitCmd(0x37) | value;
  }

  constexpr uint64_t setScissor(float x0, float y0, float x1, float y1) {
    return bitCmd(0x2D)
      | bitVal(floatTo10p2(x0), 55, 44)
      | bitVal(floatTo10p2(y0), 43, 32)
      | bitVal(floatTo10p2(x1), 32, 12)
      | bitVal(floatTo10p2(y1), 11, 0);
  }

  constexpr uint64_t setScissorExtend(float x0, float y0, float sizeX, float sizeY) {
    return setScissor(x0, y0, x0 + sizeX, y0 + sizeY);
  }

  constexpr uint64_t setTile(
    uint8_t tileIdx,
    uint32_t format, uint32_t texelSize, uint32_t lineLen,
    uint8_t maskS, uint8_t maskT
  )
  {
    return bitCmd(0x35)
      | bitVal(55, 53, format)
      | bitVal(52, 51, texelSize)
      | bitVal(49, 41, lineLen)

      | bitVal(26, 24, tileIdx)

      | bitVal(7, 4, maskS)
      | bitVal(17, 14, maskT)
    ;
  }

  constexpr uint64_t setTileSize(
    int tileIdx, float startU, float startV, float endU, float endV
  ) {
    return bitCmd(0x32)
      | bitVal(floatTo10p2(startU), 55, 44)
      | bitVal(floatTo10p2(startV), 43, 32)
      | bitVal(tileIdx, 26, 24)
      | bitVal(floatTo10p2(endU), 23, 12)
      | bitVal(floatTo10p2(endV), 11, 0);
  }

  constexpr uint64_t setTextureImage(void* data, int format, int pixelSize, int width)
  {
    return bitCmd(0x3D)
      | bitVal(format, 61, 53)
      | bitVal(pixelSize, 52, 51)
      | bitVal((width-1), 41, 32)
      | bitVal((uint64_t)data, 23, 0);
  }

  constexpr uint64_t loadBlock(int tileIdx, uint32_t tmemOffset, int texelCount, uint32_t dxt)
  {
    return bitCmd(0x33)
      | bitVal(floatTo10p2(tmemOffset), 55, 44)
      | bitVal(tileIdx, 26, 24)
      | bitVal(floatTo10p2(texelCount), 23, 12)
      //| bitVal(floatTo1p11(dxt), 11, 0);
      | bitVal(dxt, 11, 0);
  }

  constexpr uint64_t fillRectFP(int x0, int y0, int x1, int y1) {
    return bitCmd(0x36)
      | bitVal(x1, 55, 44)
      | bitVal(y1, 43, 32)
      | bitVal(x0, 32, 12)
      | bitVal(y0, 11, 0);
  }

  constexpr uint64_t fillRect(float x0, float y0, float x1, float y1) {
    return bitCmd(0x36)
      | bitVal(floatTo10p2(x1), 55, 44)
      | bitVal(floatTo10p2(y1), 43, 32)
      | bitVal(floatTo10p2(x0), 32, 12)
      | bitVal(floatTo10p2(y0), 11, 0);
  }

  constexpr uint64_t fillRectSize(float x0, float y0, float sx, float sy) {
    return fillRect(x0, y0, x0 + sx, y0 + sy);
  }

  constexpr uint64_t texRect0(uint8_t tileIdx, float x0, float y0, float x1, float y1) {
    return bitCmd(0x24)
      | bitVal(floatTo10p2(x1), 55, 44)
      | bitVal(floatTo10p2(y1), 43, 32)
      | bitVal(tileIdx, 26, 24)
      | bitVal(floatTo10p2(x0), 23, 12)
      | bitVal(floatTo10p2(y0), 11, 0);
  }
  constexpr uint64_t texRect1(float u, float v, float deltaU, float deltaV) {
    return bitVal(floatTo10p5(u), 63, 48)
      | bitVal(floatTo10p5(v), 47, 32)
      | bitVal(floatTo5p10(deltaU), 31, 16)
      | bitVal(floatTo5p10(deltaV), 15, 0);
  }

  constexpr uint64_t setOtherModes(uint64_t otherMode) {
    return bitCmd(0x2F) | otherMode;
  }

  constexpr uint64_t setCC(uint64_t cc) {
    return bitCmd(0x3C) | cc;
  }

  constexpr uint64_t setCC2Cycle(CCCycle cc0, CCCycle cc1) {
    return bitCmd(0x3C)
      | bitVal(cc0.colorA, 55, 52)
      | bitVal(cc0.colorC, 51, 47)
      | bitVal(cc0.alphaA, 46, 44)
      | bitVal(cc0.alphaC, 43, 41)
      | bitVal(cc1.colorA, 40, 37)
      | bitVal(cc1.colorC, 36, 32)
      | bitVal(cc0.colorB, 31, 28)
      | bitVal(cc1.colorB, 27, 24)
      | bitVal(cc1.alphaA, 23, 21)
      | bitVal(cc1.alphaC, 20, 18)
      | bitVal(cc0.colorD, 17, 15)
      | bitVal(cc0.alphaB, 14, 12)
      | bitVal(cc0.alphaD, 11, 9)
      | bitVal(cc1.colorD, 8, 6)
      | bitVal(cc1.alphaB, 5, 3)
      | bitVal(cc1.alphaD, 2, 0);
  }

  constexpr uint64_t setCC1Cycle(CCCycle cc0) {
    return setCC2Cycle(cc0, cc0);
  }

  constexpr uint64_t setPrimColor(uint32_t color, uint8_t  minLOD = 0, uint8_t primLOD = 0) {
    return bitCmd(0x3A)
      | bitVal(minLOD, 47, 40)
      | bitVal(primLOD, 39, 32)
      | bitVal(color, 31, 0);
  }

  constexpr uint64_t setPrimColor(color_t color, uint8_t  minLOD = 0, uint8_t primLOD = 0) {
    return setPrimColor(std::bit_cast<uint32_t>(color), minLOD, primLOD);
  }

  constexpr uint64_t setBlendColor(uint32_t color) {
    return bitCmd(0x39) | bitVal(color, 31, 0);
  }

  constexpr uint64_t setEnvColor(uint32_t color) {
    return bitCmd(0x3B) | bitVal(color, 31, 0);
  }

  constexpr uint64_t setEnvColor(const color_t &color) {
    return setEnvColor(std::bit_cast<uint32_t>(color));
  }

  TriParams triangleGen(uint32_t attrs, const Vertex &v0, const Vertex &v1, const Vertex &v2);
  std::vector<uint64_t> triangleWrite(TriParams &p, uint32_t attrs = TriAttr::POS);

  std::vector<uint64_t> triangle(uint32_t attrs, const Vertex &v0, const Vertex &v1, const Vertex &v2);

  constexpr uint64_t syncPipe() { return bitCmd(0xE7); }
  constexpr uint64_t syncFull() { return bitCmd(0x29); }

  constexpr uint64_t nop() {
    return 0;
  }

  enum class DitherRGB : uint32_t { MAGIC_4x4 = 0, BAYER_4x4, RANDOM, DISABLED };
  enum class DitherAlpha : uint32_t { RGB = 0, RGB_INV, RANDOM, DISABLED };

  class OtherMode {
    uint64_t value{0};
    public:

    constexpr OtherMode& atomic(bool enabled) {
      value |= bitVal(enabled ? 1 : 0, 55, 55); return *this;
    }
    constexpr OtherMode& cycleType(uint32_t cycleType) {
      value |= bitVal(cycleType, 53, 52); return *this;
    }
    constexpr OtherMode& ditherRGBA(DitherRGB dither) {
      value |= bitVal((uint32_t)dither, 39, 38); return *this;
    }
    constexpr OtherMode& ditherAlpha(DitherAlpha dither) {
      value |= bitVal((uint32_t)dither, 37, 36); return *this;
    }
    constexpr OtherMode& setAA(bool enable) {
      value |= bitVal(enable ? 1 : 0, 3, 3); return *this;
    }
    constexpr OtherMode& forceBlend(uint32_t mode) {
      value |= bitVal(mode, 14, 14); return *this;
    }
    constexpr OtherMode& useCoverageAsAlpha(bool enable) {
      value |= bitVal(enable ? 1 : 0, 13, 16); return *this;
    }
    constexpr OtherMode& setBlender(uint64_t blend) {
      value |= blend; return *this;
    }

    operator uint64_t() const { return value; }
  };
}