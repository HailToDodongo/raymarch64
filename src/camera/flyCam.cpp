/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "flyCam.h"
#include "../math/mathFloat.h"

namespace
{
  constexpr float MOVE_SPEED = 0.05f;
  constexpr float ROT_SPEED = 0.03f;

  constexpr float lerp_angle(float a, float b, float t) {
    float angleDiff = fmodf((b - a), FM_PI*2);
    float shortDist = fmodf(angleDiff*2, FM_PI*2) - angleDiff;
    return a + shortDist * t;
  }
}

void FlyCam::update(float deltaTime)
{
  float smoothFactor = 0.25f;

  float camSpeed = deltaTime * MOVE_SPEED;
  float camRotSpeed = deltaTime * ROT_SPEED;

  camRotXCurr = lerp_angle(camRotXCurr, camRotX, smoothFactor);
  camRotYCurr = lerp_angle(camRotYCurr, camRotY, smoothFactor);

  camDir = Math::normalize({
    fm_cosf(camRotXCurr) * fm_cosf(camRotYCurr),
    fm_sinf(camRotYCurr),
    fm_sinf(camRotXCurr) * fm_cosf(camRotYCurr),
  });

  auto joypad = joypad_get_inputs(JOYPAD_PORT_1);

  if(joypad.btn.d_up) {
    smoothFactor = 0.04f;
  }

  if(joypad.stick_x < 10 && joypad.stick_x > -10)joypad.stick_x = 0;
  if(joypad.stick_y < 10 && joypad.stick_y > -10)joypad.stick_y = 0;

  auto camDirXZ = Math::normalize({camDir.x, 0, camDir.z});

  if(joypad.btn.z) {
    camRotX += (float)joypad.stick_x * camRotSpeed;
    camRotY -= (float)joypad.stick_y * camRotSpeed;
  } else {
    camPos += camDir * ((float)joypad.stick_y * camSpeed);
    camPos.v[0] += camDir.v[2] * (float)joypad.stick_x * -camSpeed;
    camPos.v[2] -= camDir.v[0] * (float)joypad.stick_x * -camSpeed;
  }

  float cSide = (float)(joypad.btn.c_left * -32)
              + (float)(joypad.btn.c_right * 32);
  float cFwd = (float)(joypad.btn.c_up *  32)
             + (float)(joypad.btn.c_down * -32);

  camPos += camDirXZ * cFwd * camSpeed;
  camPos.v[0] += camDirXZ.v[2] * cSide * -camSpeed;
  camPos.v[2] -= camDirXZ.v[0] * cSide * -camSpeed;

  camRotY = fmaxf(camRotY, 1.6f);
  camRotY = fminf(camRotY, 4.71f);

  /*fm_vec3_lerp(cam.pos, cam.pos, camPos, smoothFactor);
  auto actualTarget = cam.pos + camDir;
  cam.target = actualTarget;*/
}