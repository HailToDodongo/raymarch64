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
  debug_init_isviewer();
  debug_init_usblog();

  dfs_init(DFS_DEFAULT_LOCATION);

  joypad_init();

  vi_init();
  vi_set_dedither(false);
  vi_set_aa_mode(VI_AA_MODE_RESAMPLE);
  vi_set_interlaced(false);
  vi_set_divot(false);
  vi_set_gamma(VI_GAMMA_DISABLE);
  wait_ms(14);

  /*disable_interrupts();
    register_VI_handler(on_vi_frame_ready);
    set_VI_interrupt(1, VI_V_CURRENT_VBLANK);
  enable_interrupts();*/

  surface_t fbs[3] = {
    surface_make((char*)0xA0280000, FMT_RGBA16, SCREEN_WIDTH, SCREEN_HEIGHT, FB_STRIDE),
    surface_make((char*)0xA0380000, FMT_RGBA16, SCREEN_WIDTH, SCREEN_HEIGHT, FB_STRIDE),
    surface_make((char*)0xA0400000, FMT_RGBA16, SCREEN_WIDTH, SCREEN_HEIGHT, FB_STRIDE),
  };
  vi_show(&fbs[1]);

  RayMarch::init();
  uint32_t frame = 0;
  float time = 0;

  for(;;) 
  {
    ++frame;
    //state.fb = &fbs[frame % 3];
    auto fb = &fbs[1];
    Text::setFrameBuffer(*fb);

    joypad_poll();

    /*while(freeFB == 0) {
      vi_wait_vblank();
    }*/

    time += 0.025f;

    disable_interrupts();
    //freeFB -= 1;
    RayMarch::draw(fb->buffer, time);
    enable_interrupts();

    //vi_show(fb);
    //vi_wait_vblank();
  }
}
