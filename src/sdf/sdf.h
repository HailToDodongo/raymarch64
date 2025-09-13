/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once

#pragma GCC push_options
#pragma GCC optimize ("-O3")
#pragma GCC optimize ("-ffast-math")

namespace SDF {
  float cube(const fm_vec3_t& p) {
    constexpr float s = 0.5f/4.0f;
    fm_vec3_t d{
      fabsf(p.x + 0.25f) - s,
      fabsf(p.y + 0.125f) - s,
      fabsf(p.z + 0.25f) - s,
    };

    d = {
      std::max(d.x, 0.0f),
      std::max(d.y, 0.0f),
      std::max(d.z, 0.0f)
    };

    float distSq = Math::dot(d, d);
    return std::sqrt(distSq);
  }

  float cylinder(const fm_vec3_t& p) {
      float r = 0.2f + (p.y*p.y);
      constexpr float h = 0.25f;
      float d[2]{ sqrtf(p.x*p.x + p.z*p.z) - r, fabsf(p.y) - h };
      d[0] = std::max(d[0], 0.0f);
      d[1] = std::max(d[1], 0.0f);
      return std::sqrt(d[0]*d[0] + d[1]*d[1]);
  }

  fm_vec3_t mainNormals(const fm_vec3_t& p_)
  {
      /*assert(p.x > -0.5f);
      assert(p.y > -0.5f);
      assert(p.z > -0.5f);

      assert(p.x < 0.5f);
      assert(p.y < 0.5f);
      assert(p.z < 0.5f);*/

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
}

#pragma GCC pop_options