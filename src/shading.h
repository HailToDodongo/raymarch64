/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once

struct TexPixel
{
  int8_t normA;
  int8_t normB;
  uint16_t color;
};
constexpr int TEX_DIM = 256;

inline uint32_t shadeResultA(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
{
  float distNorm = (renderDist - dist);
  float distNormInv = distNorm * renderDistInv;

  //uint8_t colr = (res.dist / RENDER_DIST) * 255.0f;
  //return color_to_packed16({colr,colr,colr});

  float light = -Math::dot(norm, dir);
  light = fmaxf(light, 0);

  int phase = (int)(hitPos.x+0.55f) * (int)(hitPos.z+0.55f);

  fm_vec3_t col;

  //col.x = 1.0f - distNormInv;
  if(phase & 1) {
    switch(phase & 0b110) {
      case 0b000: col = {31.0f, 10.0f, 10.0f}; break;
      case 0b010: col = {10.0f, 31.0f, 10.0f}; break;
      case 0b100: col = {31.0f, 31.0f, 10.0f}; break;
      case 0b110: col = {31.0f, 31.0f, 31.0f}; break;
    }
    light *= light;
  } else {
    light = 1.0f -light;
    light *= light;
    light = 0.2f + (light * (1.0f - 0.2f));

    col = fm_vec3_t{norm.x * 15.0f, norm.y * 15.0f, 15.0f} + 15.0f;
  }

  col *= (distNormInv * light);

  return ((int)(col.x) << 11) |
         ((int)(col.y) << 6) |
         ((int)(col.z) << 1)
  ;
}

inline uint32_t shadeResultCylinder(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
{
  float distNorm = (renderDist - dist);
  float distNormInv = distNorm * renderDistInv;

  float light = -Math::dot(norm, dir);
  light = fmaxf(light, 0);

  float s = hitPos.y * 24;
  float base = ((int)(s) & 1) ? 5.0f : 15.0f;
  s = hitPos.y*2;

  fm_vec3_t col{
    Math::sinApprox(s + 0.0f) * base + base,
    Math::sinApprox(s + 2.0f) * base + base,
    Math::sinApprox(s + 4.0f) * base + base,
  };

  //col *= (distNormInv * light);
  col = Math::mix(
    {22.0f, 22.0f, 31.0f}, col * light, distNormInv
  );

  return ((int)(col.x) << 11) |
         ((int)(col.y) << 6) |
         ((int)(col.z) << 1)
  ;
}

inline uint32_t shadeResultFlat(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
{
  float distNorm = (renderDist - dist);
  float distNormInv = distNorm * renderDistInv;

  float light = -Math::dot(norm, dir);
  light = fmaxf(light, 0);
  light = fminf(light + 0.25f, 1);

  int phase = (int)(hitPos.x+0.5f) + (int)(hitPos.z+0.5f) + (int)(hitPos.y+0.5f);
  float s = phase * 32;

  constexpr float base = 15.5f;

  fm_vec3_t col{
    Math::sinApprox(s + 0.0f) * base + base,
    Math::sinApprox(s + 2.0f) * base + base,
    Math::sinApprox(s + 4.0f) * base + base,
  };

  col = Math::mix(
    {31.0f, 11.0f, 11.0f}, col * light, distNormInv
  );

  return ((int)(col.x) << 11) |
         ((int)(col.y) << 6) |
         ((int)(col.z) << 1)
  ;
}

inline uint32_t shadeResultPointLight(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
{
  float distNorm = (renderDist - dist);
  float distNormInv = distNorm * renderDistInv;

  constexpr float LIGHT_RANGE = 1.0f / 5.0f;
  fm_vec3_t toLight = lightPos - hitPos;
  float ptLightDist = Math::length(toLight);
  ptLightDist *= LIGHT_RANGE;
  ptLightDist *= ptLightDist;

  float lightPoint = Math::dot(norm, Math::normalizeUnsafe(toLight));
  lightPoint = fmaxf(lightPoint, 0);
  lightPoint = fminf(lightPoint + 0.0125f, 1);
  lightPoint *= (1.0f - Math::clamp(ptLightDist, 0.0f, 1.0f));

  float light = lightPoint;

  constexpr float base = 15.5f;

  fm_vec3_t col;

  int phase = (int)(hitPos.x+0.5f) + (int)(hitPos.z+0.5f) + (int)(hitPos.y+0.5f);
  switch(phase & 0b110) {
      default:
      case 0b000: col = {31.0f, 15.0f, 15.0f}; break;
      case 0b010: col = {15.0f, 31.0f, 15.0f}; break;
      case 0b100: col = {31.0f, 31.0f, 15.0f}; break;
      case 0b110: col = {31.0f, 31.0f, 31.0f}; break;
    }

  /*col = Math::mix(
    {31.0f, 11.0f, 11.0f}, col * light, distNormInv
  );*/
  col *= (light * distNormInv);

  return ((int)(col.x) << 11) |
         ((int)(col.y) << 6) |
         ((int)(col.z) << 1)
  ;
}

inline fm_vec3_t rotVecY(const fm_vec3_t &v, float c, float s)
{
  return {
    v.x * c - v.z * s,
    v.y,
    v.x * s + v.z * c,
  };
}

inline fm_vec3_t rotVecY(const fm_vec3_t &v, float angle)
{
  return rotVecY(v, fm_cosf(angle), fm_sinf(angle));
}

constinit int8_t normalZLookup[256][256]{};

inline uint32_t shadeResultTex(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
{
  float distNorm = (renderDist - dist);
  float distNormInv = distNorm * renderDistInv;

  int phase = (int)(hitPos.x+0.5f) + (int)(hitPos.z+0.5f);

  float angleY = fm_atan2f(norm.z, norm.x);
  constexpr float ANGLE_TO_UV = (1.0f / (2.0f * 3.124f)) * (TEX_DIM * -2.0f);

  // Texturing
  float uv[2] {
    angleY * ANGLE_TO_UV,
    hitPos.y * (1.2f * TEX_DIM),
  };

  int uvPixel[2] = {
    (int)(uv[0]) & (TEX_DIM-1),
    (int)(uv[1]) & (TEX_DIM-1),
  };

  TexPixel* texData;
  switch (phase & 0b11) {
    default:
    case 0: texData = (TexPixel*)MemMap::TEX0_CACHED; break;
    case 1: texData = (TexPixel*)MemMap::TEX1_CACHED; break;
    case 2: texData = (TexPixel*)MemMap::TEX2_CACHED; break;
  }

  const TexPixel& tex = texData[uvPixel[1] * TEX_DIM + uvPixel[0]];

  fm_vec3_t col;
  col.x = (tex.color >> 11);
  col.y = (tex.color >> 6) & 0x1F;
  col.z = (tex.color) & 0x1F;

  fm_vec3_t normTex = {
    (tex.normA * (1.0f / 128.0f)),
    (tex.normB * (1.0f / 128.0f)),
    0,
  };

  //auto held = joypad_get_buttons_held(JOYPAD_PORT_1);

  if (tex.color & (1<<5)) { // marks the Z component to be 1
    // simplified version of the rotate assuming Z=1
    normTex.z = 1.0f;
    normTex = rotVecY(normTex, norm.x, norm.z);
  } else {
    float sq = normTex.x*normTex.x - normTex.y*normTex.y;
    if (sq < 0.95f) {
      normTex.z = sqrtf(1.0f - sq);
    }
    normTex = rotVecY(normTex, norm.x, norm.z);
  }

  constexpr auto ambientColor = fm_vec3_t{0.15f, 0.15f, 0.3f};

  float lightPoint = Math::dot(normTex, lightPos);
  lightPoint = fmaxf(lightPoint, 0);

  fm_vec3_t lightColor = fm_vec3_t{1.0f, 0.8f, 0.6f} * lightPoint;
  lightColor = Math::min(lightColor + ambientColor, {1,1,1});
  col *= lightColor;
  col *= distNormInv;

  return ((int)(col.x) << 11) |
         ((int)(col.y) << 6) |
         ((int)(col.z) << 1)
  ;
}

inline uint32_t shadeResultEnv(const fm_vec3_t &norm, const fm_vec3_t &hitPos, const fm_vec3_t &dir, float dist)
{
  float distNorm = (renderDist - dist);
  float distNormInv = distNorm * renderDistInv;
  distNormInv = fminf((distNormInv * 2.0f), 1.0f);

  // transform normal to screenspace normal
  float normX = -Math::dot(norm, right) * 0.4f + 0.5f;
  float normY = Math::dot(norm, up) * 0.4f + 0.5f;

  float angle = 1.0f - (Math::dot(norm, dir) * 0.5f + 0.5f);

  // Texturing
  float uv[2] {
    normX * TEX_DIM,
    normY * TEX_DIM,
  };

  int uvPixel[2] = {
    (int)(uv[0]) & (TEX_DIM-1),
    (int)(uv[1]) & (TEX_DIM-1),
  };

  TexPixel* texData = (TexPixel*)(MemMap::TEX3_CACHED);
  const TexPixel& tex = texData[uvPixel[1] * TEX_DIM + uvPixel[0]];

  fm_vec3_t col;
  col.x = (tex.color >> 11);
  col.y = (tex.color >> 6) & 0x1F;
  col.z = (tex.color) & 0x1F;

  constexpr fm_vec3_t fresnelCol{31.0f, 31.0f, 31.0f};
  col = Math::mix(
    fresnelCol, col, (angle * distNormInv)
  );

//  col *= (distNormInv);

  return ((int)(col.x) << 11) |
         ((int)(col.y) << 6) |
         ((int)(col.z) << 1)
  ;
}
