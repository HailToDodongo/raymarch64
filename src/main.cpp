/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include <libdragon.h>
#include "text.h"
#include "main.h"
#include "raymarch.h"

namespace {
  volatile int freeFB = 3;
  void on_vi_frame_ready()
  {
    disable_interrupts();
    if(freeFB < 3) {
      freeFB += 1;
    }
    enable_interrupts();
  }
}

[[noreturn]]
int main()
{
  //debug_init_isviewer();
  //debug_init_usblog();
  //dfs_init(DFS_DEFAULT_LOCATION);

  joypad_init();

  vi_init();
  vi_set_dedither(false);
  vi_set_aa_mode(VI_AA_MODE_RESAMPLE);
  vi_set_interlaced(false);
  vi_set_divot(false);
  vi_set_gamma(VI_GAMMA_DISABLE);

  /*
  disable_interrupts();
    register_VI_handler(on_vi_frame_ready);
    set_VI_interrupt(1, VI_V_CURRENT_VBLANK);
  enable_interrupts();
  */

  surface_t fbs[3] = {
    surface_make((char*)0xA0280000, FMT_RGBA16, SCREEN_WIDTH, SCREEN_HEIGHT, FB_STRIDE),
    surface_make((char*)0xA0300000, FMT_RGBA16, SCREEN_WIDTH, SCREEN_HEIGHT, FB_STRIDE),
    surface_make((char*)0xA0380000, FMT_RGBA16, SCREEN_WIDTH, SCREEN_HEIGHT, FB_STRIDE),
  };

  RayMarch::init();
  uint32_t frame = 0;
  float time = 0;
  bool lowRes = false;
  int redrawMenu = 4;

  constexpr int MAX_SDF_IDX = 3;
  int sdfIdx = 0;

  wait_ms(500);
  vi_show(&fbs[0]);

  for(;;) 
  {
    joypad_poll();
    auto press = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    if(press.a) { lowRes = !lowRes; redrawMenu = 4; }

    if(press.l) { --sdfIdx; redrawMenu = 4; time = 0; }
    if(press.r) { ++sdfIdx; redrawMenu = 4; time = 0; }

    if(sdfIdx < 0)sdfIdx = MAX_SDF_IDX;
    if(sdfIdx > MAX_SDF_IDX)sdfIdx = 0;

    if(lowRes) {
      /*while(freeFB == 0) {
        vi_wait_vblank();
      }*/
      frame = (frame + 1) % 3;
    }

    auto fb = &fbs[frame];
    Text::setFrameBuffer(*fb);

    if(redrawMenu != 0) {
      Text::printf(130, 222, "[L/R] SDF:%d", sdfIdx);
      Text::print(240, 222, lowRes ? "[A] 1/4x" : "[A] 1x``");
      --redrawMenu;
    }

    time += 0.025f;

    disable_interrupts();
    //freeFB -= 1;
    auto ticks = get_ticks();
    RayMarch::draw(fb->buffer, time, sdfIdx, lowRes);
    ticks = get_ticks() - ticks;
    enable_interrupts();

    Text::printf(16, 222, "%.2fms``", TICKS_TO_US(ticks) * (1.0f / 1000.0f));

    vi_show(fb);
    //vi_wait_vblank();
  }
}
