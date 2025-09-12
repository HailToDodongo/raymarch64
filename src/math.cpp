/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "math.h"

namespace {
  constexpr float pi_hi = 3.14159274f; // 0x1.921fb6p+01
  constexpr float pi_lo = -8.74227766e-08f; // -0x1.777a5cp-24
}

float Math::sinApprox(float x) {
  x = fm_fmodf(x+pi_hi, 2*pi_hi) - pi_hi;
  float p, s;
  s = x * x;
  p = 6.6208802163e-3f,
  p *= s;
  p += - 1.0132116824e-1f;

  x = x * ((x - pi_hi) - pi_lo) * ((x + pi_hi) + pi_lo) * p;
  return x;
}