/**
* @copyright 2024 - Max Bebök
* @license MIT
*/
#include "rdp.h"
#include <limits>

using namespace RDP;

namespace
{
  #define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

  constexpr float FLT_MIN = std::numeric_limits<float>::min();
  constexpr float FLT_MAX = std::numeric_limits<float>::max();

  uint32_t float_to_s16_16(float f)
  {
      if(f >= 32768.f)return 0x7FFFFFFF;
      if(f < -32768.f)return 0x80000000;
      return std::bit_cast<uint32_t>((int32_t)fm_floorf(f * 65536.f));
  }

  struct rdpq_tri_edge_data_t {
    float hx{0};               ///< High edge (X)
    float hy{0};               ///< High edge (Y)
    float mx{0};               ///< Middle edge (X)
    float my{0};               ///< Middle edge (Y)
    float fy{0};               ///< Fractional part of Y1 (top vertex)
    float ish{0};              ///< Inverse slope of higher edge
    float attr_factor{0};      ///< Inverse triangle normal (used to calculate gradients)
  };

  RDP::TriParams rdpq_write_edge_coeffs(rdpq_tri_edge_data_t &data, uint8_t tile, uint8_t mipmaps, const float *v1, const float *v2, const float *v3)
  {
    TriParams p{};

    const float x1 = v1[0];
    const float x2 = v2[0];
    const float x3 = v3[0];
    const float y1 = fm_floorf(v1[1]*4)/4;
    const float y2 = fm_floorf(v2[1]*4)/4;
    const float y3 = fm_floorf(v3[1]*4)/4;

    const float to_fixed_11_2 = 4.0f;
    p.y1f = CLAMP((int32_t)fm_floorf(v1[1]*to_fixed_11_2), -4096*4, 4095*4);
    p.y2f = CLAMP((int32_t)fm_floorf(v2[1]*to_fixed_11_2), -4096*4, 4095*4);
    p.y3f = CLAMP((int32_t)fm_floorf(v3[1]*to_fixed_11_2), -4096*4, 4095*4);

    data.hx = x3 - x1;
    data.hy = y3 - y1;
    data.mx = x2 - x1;
    data.my = y2 - y1;
    float lx = x3 - x2;
    float ly = y3 - y2;

    const float nz = (data.hx*data.my) - (data.hy*data.mx);
    data.attr_factor = (abs(nz) > FLT_MIN) ? (-1.0f / nz) : 0;
    p.lft = nz < 0 ? 1 : 0;

    p.ish = data.ish = (abs(data.hy) > FLT_MIN) ? (data.hx / data.hy) : 0;
    p.ism = (abs(data.my) > FLT_MIN) ? (data.mx / data.my) : 0;
    p.isl = (abs(ly) > FLT_MIN) ? (lx / ly) : 0;
    data.fy = fm_floorf(y1) - y1;

    p.xh = x1 + data.fy * data.ish;
    p.xm = x1 + data.fy * p.ism;
    p.xl = x2;
    return p;
/*
    debugf("x1:  %f (%08lx)\n", x1, (int32_t)(x1 * 4.0f));
    debugf("x2:  %f (%08lx)\n", x2, (int32_t)(x2 * 4.0f));
    debugf("x3:  %f (%08lx)\n", x3, (int32_t)(x3 * 4.0f));
    debugf("y1:  %f (%08lx)\n", y1, (int32_t)(y1 * 4.0f));
    debugf("y2:  %f (%08lx)\n", y2, (int32_t)(y2 * 4.0f));
    debugf("y3:  %f (%08lx)\n", y3, (int32_t)(y3 * 4.0f));

    debugf("hx:  %f (%08lx)\n", data.hx, (int32_t)(data.hx * 4.0f));
    debugf("hy:  %f (%08lx)\n", data.hy, (int32_t)(data.hy * 4.0f));
    debugf("mx:  %f (%08lx)\n", data.mx, (int32_t)(data.mx * 4.0f));
    debugf("my:  %f (%08lx)\n", data.my, (int32_t)(data.my * 4.0f));
    debugf("lx:  %f (%08lx)\n", lx, (int32_t)(lx * 4.0f));
    debugf("ly:  %f (%08lx)\n", ly, (int32_t)(ly * 4.0f));

    debugf("p1: %f (%08lx)\n", (data.hx*data.my), (int32_t)(data.hx*data.my*16.0f));
    debugf("p2: %f (%08lx)\n", (data.hy*data.mx), (int32_t)(data.hy*data.mx*16.0f));
    debugf("nz: %f (%08lx)\n", nz, (int32_t)(nz * 16.0f));
    debugf("-nz: %f (%08lx)\n", -nz, -(int32_t)(nz * 16.0f));
    debugf("inv_nz: %f (%08lx)\n", data.attr_factor, (int32_t)(data.attr_factor * 65536.0f / 2.0f / 16.0f));

    debugf("fy:  %f (%08lx)\n", data.fy, (int32_t)(data.fy * 65536.0f));
    debugf("ish: %f (%08lx)\n", data.ish, (int32_t)(data.ish * 65536.0f));
    debugf("ism: %f (%08lx)\n", ism, (int32_t)(ism * 65536.0f));
    debugf("isl: %f (%08lx)\n", isl, (int32_t)(isl * 65536.0f));

    debugf("xh: %f (%08lx)\n", xh, (int32_t)(xh * 65536.0f));
    debugf("xm: %f (%08lx)\n", xm, (int32_t)(xm * 65536.0f));
    debugf("xl: %f (%08lx)\n", xl, (int32_t)(xl * 65536.0f));*/
  }

  void rdpq_write_shade_coeffs(std::vector<uint64_t> &out, const TriParams &p)
  {
    const uint32_t DrDx_fixed = float_to_s16_16(p.DrgbaDx[0]);
    const uint32_t DgDx_fixed = float_to_s16_16(p.DrgbaDx[1]);
    const uint32_t DbDx_fixed = float_to_s16_16(p.DrgbaDx[2]);
    const uint32_t DaDx_fixed = float_to_s16_16(p.DrgbaDx[3]);

    const uint32_t DrDe_fixed = float_to_s16_16(p.DrgbaDe[0]);
    const uint32_t DgDe_fixed = float_to_s16_16(p.DrgbaDe[1]);
    const uint32_t DbDe_fixed = float_to_s16_16(p.DrgbaDe[2]);
    const uint32_t DaDe_fixed = float_to_s16_16(p.DrgbaDe[3]);

    const uint32_t DrDy_fixed = float_to_s16_16(p.DrgbaDy[0]);
    const uint32_t DgDy_fixed = float_to_s16_16(p.DrgbaDy[1]);
    const uint32_t DbDy_fixed = float_to_s16_16(p.DrgbaDy[2]);
    const uint32_t DaDy_fixed = float_to_s16_16(p.DrgbaDy[3]);

    auto write = [&out](uint32_t high, uint32_t low) {
      out.push_back(((uint64_t)high << 32) | (uint64_t)low);
    };

    write((p.final_rgba[0]&0xffff0000) | (0xffff&(p.final_rgba[1]>>16)),
          (p.final_rgba[2]&0xffff0000) | (0xffff&(p.final_rgba[3]>>16)));
    write((DrDx_fixed&0xffff0000) | (0xffff&(DgDx_fixed>>16)),
          (DbDx_fixed&0xffff0000) | (0xffff&(DaDx_fixed>>16)));
    write((p.final_rgba[0]<<16) | (p.final_rgba[1]&0xffff),
          (p.final_rgba[2]<<16) | (p.final_rgba[3]&0xffff));
    write((DrDx_fixed<<16) | (DgDx_fixed&0xffff),
          (DbDx_fixed<<16) | (DaDx_fixed&0xffff));
    write((DrDe_fixed&0xffff0000) | (0xffff&(DgDe_fixed>>16)),
          (DbDe_fixed&0xffff0000) | (0xffff&(DaDe_fixed>>16)));
    write((DrDy_fixed&0xffff0000) | (0xffff&(DgDy_fixed>>16)),
          (DbDy_fixed&0xffff0000) | (0xffff&(DaDy_fixed>>16)));
    write((DrDe_fixed<<16) | (DgDe_fixed&0xffff),
          (DbDe_fixed<<16) | (DaDe_fixed&0xffff));
    write((DrDy_fixed<<16) | (DgDy_fixed&0xffff),
            (DbDy_fixed<<16) | (DaDy_fixed&0xffff));

    /*debugf("b1: %f (%08lx)\n", v1[2], (uint32_t)(v1[2]*255.0f));
    debugf("b2: %f (%08lx)\n", v2[2], (uint32_t)(v2[2]*255.0f));
    debugf("b3: %f (%08lx)\n", v3[2], (uint32_t)(v3[2]*255.0f));
    debugf("mb: %f (%08lx)\n", mb, (uint32_t)(int32_t)mb);
    debugf("hb: %f (%08lx)\n", hb, (uint32_t)(int32_t)hb);
    debugf("nxB: %f (%08lx)\n", nxB, (int32_t)(nxB * 4.0f));
    debugf("DbDx: %f (%08lx)\n", DbDx, (uint32_t)(DbDx * 65536.0f));
    debugf("DbDx_fixed: (%08lx)\n", DbDx_fixed);*/
  }
}

RDP::TriParams RDP::triangleGen(uint32_t attrs, const Vertex &v0, const Vertex &v1, const Vertex &v2) {
  const Vertex *v[3] = {&v0, &v1, &v2};
  if(v[0]->pos[1] > v[1]->pos[1])std::swap(v[0], v[1]);
  if(v[1]->pos[1] > v[2]->pos[1])std::swap(v[1], v[2]);
  if(v[0]->pos[1] > v[1]->pos[1])std::swap(v[0], v[1]);

  rdpq_tri_edge_data_t data{};
  auto p = rdpq_write_edge_coeffs(data, 0, 0, v[0]->pos, v[1]->pos, v[2]->pos);

  if(attrs & TriAttr::SHADE)
  {
    const float mr = (v1.color[0] - v0.color[0]) * 255.f;
    const float mg = (v1.color[1] - v0.color[1]) * 255.f;
    const float mb = (v1.color[2] - v0.color[2]) * 255.f;
    const float ma = (v1.color[3] - v0.color[3]) * 255.f;
    const float hr = (v2.color[0] - v0.color[0]) * 255.f;
    const float hg = (v2.color[1] - v0.color[1]) * 255.f;
    const float hb = (v2.color[2] - v0.color[2]) * 255.f;
    const float ha = (v2.color[3] - v0.color[3]) * 255.f;

    const float nxR = data.hy*mr - data.my*hr;
    const float nxG = data.hy*mg - data.my*hg;
    const float nxB = data.hy*mb - data.my*hb;
    const float nxA = data.hy*ma - data.my*ha;
    const float nyR = data.mx*hr - data.hx*mr;
    const float nyG = data.mx*hg - data.hx*mg;
    const float nyB = data.mx*hb - data.hx*mb;
    const float nyA = data.mx*ha - data.hx*ma;

    p.DrgbaDx[0] = nxR * data.attr_factor;
    p.DrgbaDx[1] = nxG * data.attr_factor;
    p.DrgbaDx[2] = nxB * data.attr_factor;
    p.DrgbaDx[3] = nxA * data.attr_factor;

    p.DrgbaDy[0] = nyR * data.attr_factor;
    p.DrgbaDy[1] = nyG * data.attr_factor;
    p.DrgbaDy[2] = nyB * data.attr_factor;
    p.DrgbaDy[3] = nyA * data.attr_factor;

    for(int i=0; i<4; ++i) {
      p.DrgbaDe[i] = p.DrgbaDy[i] + p.DrgbaDx[i] * data.ish;
      p.final_rgba[i] = float_to_s16_16(v0.color[i] * 255.f + data.fy * p.DrgbaDe[i]);
    }
  }
  return p;
}

std::vector<uint64_t> RDP::triangleWrite(TriParams &p, uint32_t attrs) {
  std::vector<uint64_t> out;

  uint32_t cmd = 0x08;
  if(attrs & TriAttr::SHADE)cmd |= 0x04;

  uint64_t cmdPos = bitCmd(cmd);
  out.push_back(cmdPos);

  bool mipmaps = false;
  uint8_t tile = 0;

  uint32_t out0 = _carg(p.lft, 0x1, 23) | _carg(mipmaps ? mipmaps-1 : 0, 0x7, 19) | _carg(tile, 0x7, 16) | _carg(p.y3f, 0x3FFF, 0);
  uint32_t out1 = (_carg(p.y2f, 0x3FFF, 16) | _carg(p.y1f, 0x3FFF, 0));
  out[0] |= ((uint64_t)out0 << 32) | (uint64_t)out1;

  out.push_back(((uint64_t)float_to_s16_16(p.xl) << 32) | (uint64_t)float_to_s16_16(p.isl));
  out.push_back(((uint64_t)float_to_s16_16(p.xh) << 32) | (uint64_t)float_to_s16_16(p.ish));
  out.push_back(((uint64_t)float_to_s16_16(p.xm) << 32) | (uint64_t)float_to_s16_16(p.ism));

  if(attrs & TriAttr::SHADE) {
    rdpq_write_shade_coeffs(out, p);
  }

  return out;
}

std::vector<uint64_t> RDP::triangle(uint32_t attrs, const Vertex &v0, const Vertex &v1, const Vertex &v2) {
  auto p = triangleGen(attrs, v0, v1, v2);
  return triangleWrite(p, attrs);
}
