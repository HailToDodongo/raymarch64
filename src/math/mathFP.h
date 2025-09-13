/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once

struct FP32
{
  int32_t val{0};

  constexpr FP32() = default;
  constexpr FP32(float f) {
    val = (int32_t)(f * 0x10000);
  }
  constexpr FP32(int32_t v) : val(v) {}

  constexpr float toFloat() const {
    return (float)((float)val * (float)(1.0 / 0x10000));
  }

  constexpr FP32 floor() const {
    return { (int32_t)(val & 0xFFFF'0000) };
  }

  FP32& operator=(float f) {
    *this = FP32{f};
    return *this;
  }

  FP32 operator>>(int bits) const {
    return { val >> bits };
  }
  FP32 operator<(int bits) const {
    return { val << bits };
  }

  FP32 operator+(const FP32& o) const {
    return {val + o.val};
  }
  FP32 operator-(const FP32& o) const {
    return {val - o.val};
  }
  FP32 operator*(const FP32& o) const {
    int64_t r = (int64_t)val * (int64_t)o.val;
    r = r >> 16;
    return {(int32_t)r};
  }

  void operator+=(const FP32& o) {
    val += o.val;
  }
  void operator-=(const FP32& o) {
    val -= o.val;
  }

  bool operator<(const FP32& o) const {
    return val < o.val;
  }
  bool operator<=(const FP32& o) const {
    return val <= o.val;
  }
  bool operator>(const FP32& o) const {
    return val > o.val;
  }
  bool operator>=(const FP32& o) const {
    return val >= o.val;
  }
};

struct FP32Vec3 {
  FP32 x{}, y{}, z{};

  constexpr FP32Vec3() = default;
  constexpr FP32Vec3(FP32 x, FP32 y, FP32 z) : x(x), y(y), z(z) {}

  fm_vec3_t toFmVec3() const {
    return { x.toFloat(), y.toFloat(), z.toFloat() };
  }

  FP32Vec3 floor() const {
    return { x.floor(), y.floor(), z.floor() };
  }

  FP32Vec3 operator+(const FP32Vec3& o) const {
    return { x + o.x, y + o.y, z + o.z };
  }
  FP32Vec3 operator-(const FP32Vec3& o) const {
    return { x - o.x, y - o.y, z - o.z };
  }
  FP32Vec3 operator*(const FP32& s) const {
    return { x * s, y * s, z * s };
  }
  void operator+=(const FP32Vec3& o) {
    x += o.x; y += o.y; z += o.z;
  }
  void operator-=(const FP32Vec3& o) {
    x -= o.x; y -= o.y; z -= o.z;
  }
};