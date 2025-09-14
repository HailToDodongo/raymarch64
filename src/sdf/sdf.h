/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once

#pragma GCC push_options
#pragma GCC optimize ("-O3")
#pragma GCC optimize ("-ffast-math")

namespace SDF {

  fm_vec3_t mainNormals(const fm_vec3_t& p_)
  {
     auto p = Math::fastClamp(p_);
      constexpr float r1 = 0.25f;
      float distXZ = Math::sqrtfApprox(p.x*p.x + p.z*p.z);
      float qx = 1.0f - (r1 / distXZ);
      fm_vec3_t normTorus{
        p.x * qx,
        p.y,
        p.z * qx,
      };
      return Math::normalizeUnsafe(Math::mix(normTorus, p, lerpFactor));
  }

  float main(const fm_vec3_t& p) {
      constexpr float r1 = 0.25f;
      constexpr float r2 = 0.075f;

      fm_vec3_t pSq{p.x*p.x, p.y*p.y, p.z*p.z};
      float pSqXZ = pSq.x + pSq.z;

      float distSphere = sqrtf(pSqXZ + pSq.y) - r1;
      float qx = sqrtf(pSqXZ) - r1;
      float distTorus = sqrtf(qx*qx + pSq.y) - r2;

      return Math::mix(distTorus, distSphere, lerpFactor);
  }

  float sphere(const fm_vec3_t& p) {
      constexpr float r = 0.25f;
      return sqrtf(p.x*p.x + p.y*p.y + p.z*p.z) - r;
  }

  fm_vec3_t sphereNormals(const fm_vec3_t& p) {
      return Math::normalizeUnsafe(
        Math::fastClamp(p)
      );
  }

  float cylinder(const fm_vec3_t& p) {
      constexpr float r = 0.25f;
      return sqrtf(p.x*p.x + p.z*p.z) - r;
  }

  fm_vec3_t cylinderNormals(const fm_vec3_t& p) {
      return Math::normalizeUnsafe(
        Math::fastClamp(fm_vec3_t{p.x, 0, p.z})
      );
  }

  float octa(const fm_vec3_t& p) {
    fm_vec3_t pAbs{
      fabsf(p.x),
      fabsf(p.y),
      fabsf(p.z)
    };
    float sum = pAbs.x + pAbs.y + pAbs.z;
    return (sum - lerpFactor) * 0.5773f;
  }

  fm_vec3_t octaNormals(const fm_vec3_t& p_) {
    auto p = Math::fastClamp(p_);
    // sharp flat normals
    fm_vec3_t n{
      (p.x >= 0) ? 1.0f : -1.0f,
      (p.y >= 0) ? 1.0f : -1.0f,
      (p.z >= 0) ? 1.0f : -1.0f,
    };
    return Math::normalizeUnsafe(n);
  }

}

#pragma GCC pop_options