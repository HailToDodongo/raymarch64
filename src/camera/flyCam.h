/**
* @copyright 2024 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>

struct FlyCam
{
  fm_vec3_t camDir{};
  fm_vec3_t camPos{};

  float camRotX{};
  float camRotY{};

  float camRotXCurr{};
  float camRotYCurr{};

  void update(float deltaTime);

  void setRotation(float rotX, float rotY) {
    camRotX = rotX;
    camRotY = rotY;
    camRotXCurr = rotX;
    camRotYCurr = rotY;
  }
};