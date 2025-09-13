/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>
#include <cmath>

// Operator overloads
[[maybe_unused]] inline fm_vec3_t operator+(fm_vec3_t const& lhs, fm_vec3_t const& rhs) {
  return {lhs.v[0] + rhs.v[0], lhs.v[1] + rhs.v[1], lhs.v[2] + rhs.v[2]};
}

[[maybe_unused]] inline fm_vec3_t operator-(fm_vec3_t const& lhs, fm_vec3_t const& rhs) {
  return {lhs.v[0] - rhs.v[0], lhs.v[1] - rhs.v[1], lhs.v[2] - rhs.v[2]};
}

[[maybe_unused]] inline fm_vec3_t operator*(fm_vec3_t const& lhs, fm_vec3_t const& rhs) {
  return {lhs.v[0] * rhs.v[0], lhs.v[1] * rhs.v[1], lhs.v[2] * rhs.v[2]};
}

[[maybe_unused]] inline fm_vec3_t operator/(fm_vec3_t const& lhs, fm_vec3_t const& rhs) {
  return {lhs.v[0] / rhs.v[0], lhs.v[1] / rhs.v[1], lhs.v[2] / rhs.v[2]};
}

[[maybe_unused]] inline fm_vec3_t operator+(fm_vec3_t const& lhs, float rhs) {
  return {lhs.v[0] + rhs, lhs.v[1] + rhs, lhs.v[2] + rhs};
}

[[maybe_unused]] inline fm_vec3_t operator-(fm_vec3_t const& lhs, float rhs) {
  return {lhs.v[0] - rhs, lhs.v[1] - rhs, lhs.v[2] - rhs};
}

[[maybe_unused]] inline fm_vec3_t operator*(fm_vec3_t const& lhs, float rhs) {
  return {lhs.v[0] * rhs, lhs.v[1] * rhs, lhs.v[2] * rhs};
}

[[maybe_unused]] inline fm_vec3_t operator/(fm_vec3_t const& lhs, float rhs) {
  return {lhs.v[0] / rhs, lhs.v[1] / rhs, lhs.v[2] / rhs};
}

[[maybe_unused]] inline fm_vec3_t operator-(fm_vec3_t const& lhs) {
  return {-lhs.v[0], -lhs.v[1], -lhs.v[2]};
}

[[maybe_unused]] inline fm_vec3_t& operator+=(fm_vec3_t &lhs, fm_vec3_t const& rhs) {
  lhs.v[0] += rhs.v[0];
  lhs.v[1] += rhs.v[1];
  lhs.v[2] += rhs.v[2];
  return lhs;
}

[[maybe_unused]] inline fm_vec3_t& operator+=(fm_vec3_t &lhs, float rhs) {
  lhs.v[0] += rhs;
  lhs.v[1] += rhs;
  lhs.v[2] += rhs;
  return lhs;
}

[[maybe_unused]] inline fm_vec3_t& operator-=(fm_vec3_t &lhs, fm_vec3_t const& rhs) {
  lhs.v[0] -= rhs.v[0];
  lhs.v[1] -= rhs.v[1];
  lhs.v[2] -= rhs.v[2];
  return lhs;
}

[[maybe_unused]] inline fm_vec3_t& operator-=(fm_vec3_t &lhs, float rhs) {
  lhs.v[0] -= rhs;
  lhs.v[1] -= rhs;
  lhs.v[2] -= rhs;
  return lhs;
}

[[maybe_unused]] inline fm_vec3_t& operator*=(fm_vec3_t &lhs, fm_vec3_t const& rhs) {
  lhs.v[0] *= rhs.v[0];
  lhs.v[1] *= rhs.v[1];
  lhs.v[2] *= rhs.v[2];
  return lhs;
}

[[maybe_unused]] inline fm_vec3_t& operator*=(fm_vec3_t &lhs, float rhs) {
  lhs.v[0] *= rhs;
  lhs.v[1] *= rhs;
  lhs.v[2] *= rhs;
  return lhs;
}

[[maybe_unused]] inline fm_vec3_t& operator/=(fm_vec3_t &lhs, fm_vec3_t const& rhs) {
  lhs.v[0] /= rhs.v[0];
  lhs.v[1] /= rhs.v[1];
  lhs.v[2] /= rhs.v[2];
  return lhs;
}

[[maybe_unused]] inline fm_vec3_t& operator/=(fm_vec3_t &lhs, float rhs) {
  lhs.v[0] /= rhs;
  lhs.v[1] /= rhs;
  lhs.v[2] /= rhs;
  return lhs;
}

namespace Math
{
  // sin_approx() taken and reduced from libdragons fm_sinf()
  float sinApprox(float x);

  constexpr float clamp(float v, float mn, float mx) {
    return v < mn ? mn : (v > mx ? mx : v);
  }

  inline fm_vec3_t fastClamp(const fm_vec3_t& p)
  {
    return {
      (fabsf(p.x) > 0.5f) ? (p.x - fm_floorf(p.x + 0.5f)) : p.x,
      (fabsf(p.y) > 0.5f) ? (p.y - fm_floorf(p.y + 0.5f)) : p.y,
      (fabsf(p.z) > 0.5f) ? (p.z - fm_floorf(p.z + 0.5f)) : p.z,
    };
  }

  inline float mix(float a, float b, float t) {
    return a * (1.0f - t) + b * t;
    //return (b - a) * t + a;
  }

  inline fm_vec3_t mix(const fm_vec3_t &a, const fm_vec3_t &b, float t) {
    return {
      mix(a.x, b.x, t),
      mix(a.y, b.y, t),
      mix(a.z, b.z, t),
    };
  }

  inline float sqrtfApprox(float x)
  {
    int32_t i = std::bit_cast<int32_t>(x);
    i  += 127 << 23;
    i >>= 1;
    return std::bit_cast<float>(i);
  }

  inline float dot(const fm_vec3_t& a, const fm_vec3_t& b) {
      return a.x*b.x + a.y*b.y + a.z*b.z;
  }

  inline float length(const fm_vec3_t& v) {
      return std::sqrt(dot(v,v));
  }
  inline fm_vec3_t normalize(const fm_vec3_t& v) {
      float len = length(v);

      if(len < 0.0001f) return v;
      len = 1.0f / len;
      return { v.x*len, v.y*len, v.z*len };
  }

  inline fm_vec3_t normalizeUnsafe(const fm_vec3_t& v) {
      float len = 1.0f / length(v);
      return { v.x*len, v.y*len, v.z*len };
  }

  inline fm_vec3_t cross(const fm_vec3_t& a, const fm_vec3_t& b) {
      return {
          a.y*b.z - a.z*b.y,
          a.z*b.x - a.x*b.z,
          a.x*b.y - a.y*b.x
      };
  }
}