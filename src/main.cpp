/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include <libdragon.h>
#include <initializer_list>
#include "text.h"
#include "main.h"
#include "raymarch.h"
#include "camera/flyCam.h"
#include "math/mathFloat.h"

constinit FlyCam camera{};

namespace {

  constinit uint32_t frame = 0;
  constinit float currTime = 0.0f;
  constinit int resolution = 2;
  constinit bool freeCam = true;
  constinit int redrawMenu = 4;

  constexpr int MAX_SDF_IDX = 9;
  int sdfIdx = MAX_SDF_IDX-1;

  surface_t fbs[3] = {
    {FMT_RGBA16, SCREEN_WIDTH, SCREEN_HEIGHT, FB_STRIDE, (void*)MemMap::FB0},
    {FMT_RGBA16, SCREEN_WIDTH, SCREEN_HEIGHT, FB_STRIDE, (void*)MemMap::FB1},
    {FMT_RGBA16, SCREEN_WIDTH, SCREEN_HEIGHT, FB_STRIDE, (void*)MemMap::FB2},
  };
}

extern "C" {
  volatile uint32_t resetAddr = 0;
  volatile constinit bool isReset = false;

  /**
   * Custom exception handler to ignore FPU exceptions and soft-reset instead.
   * A lot of places that do float math don't care about things like zero-divisions,
   * if they know it's safe under normal conditions.
   * This increases performance since those ops can happen multiple times per pixel.
   *
   * However in rare situations this can fail (e.g. camera origin is exactly on the surface of a SDF),
   * So to keep everything running we just jump back to main to reset.
   * The relevant state (camera, time, current SDF etc.) is kept intact.
   */
  void fpuExceptionHandler(exception_t *exc) {
    if(exc->code != EXCEPTION_CODE_FLOATING_POINT) {
      exception_default_handler(exc);
    }

    debugf("FPU Exception, reset!");
    // since the only place the exception can come from is the ray-marcher,
    // we know we had interrupts disabled
    enable_interrupts();
    asm volatile("j %0; nop" : : "r"(resetAddr) );
  }
}

[[noreturn]]
int main()
{
  resetAddr = (uint32_t)main;
  register_exception_handler(fpuExceptionHandler);
  *DP_STATUS = (DP_WSTATUS_SET_FREEZE | DP_WSTATUS_SET_FLUSH);

  if(!isReset)
  {
    isReset = true;

    joypad_init();
    debug_init_isviewer();
    debug_init_usblog();
    dfs_init(DFS_DEFAULT_LOCATION);

    // check if out manual memory map conflicts with the static data
    assert(PhysicalAddr(HEAP_START_ADDR) < PhysicalAddr((void*)MemMap::FB0));

    vi_init();
    vi_set_dedither(false);
    vi_set_aa_mode(VI_AA_MODE_RESAMPLE);
    vi_set_interlaced(false);
    vi_set_divot(false);
    vi_set_gamma(VI_GAMMA_DISABLE);

    RayMarch::init();

    camera.setRotation(0.1f, 2.8f);
    camera.camPos = {0.5f, 0.5f, 0.5f};

    // clear framebuffers
    for(auto& fb : fbs) {
      memset(fb.buffer, 0, fb.width * fb.height * 2);
    }

    vi_show(&fbs[0]);
  }

  for(;;)
  {
    float deltaTime = 0.1f / resolution;

    auto markMenuRedraw = [](){
      redrawMenu = (resolution > 1) ? 4 : 1;
    };

    joypad_poll();
    auto press = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    if(press.a || press.b) {
      if (press.a && resolution < 4)resolution *= 2;
      if (press.b && resolution > 1)resolution /= 2;
      markMenuRedraw();
      vi_wait_vblank();
    }

    if(press.l) { --sdfIdx; markMenuRedraw(); currTime = 0; }
    if(press.r) { ++sdfIdx; markMenuRedraw(); currTime = 0; }

    if(press.start)freeCam = !freeCam;

    if(sdfIdx < 0)sdfIdx = MAX_SDF_IDX;
    if(sdfIdx > MAX_SDF_IDX)sdfIdx = 0;

    if(resolution > 1) {
      // low-res mode is fast enough to afford proper buffering.
      // for high-res we intentionally keep the same buffer to see the progress in real time
      frame = (frame + 1) % 3;
    }

    if(freeCam) {
      camera.update(deltaTime);
    } else {
      float angle = (currTime + 3.5f) * 0.7f;

      camera.camPos.x = fm_sinf(angle) * 2.55f;
      camera.camPos.y = fm_cosf(angle-1.1f) * 2.45f;
      camera.camPos.z = fm_sinf(angle*0.6f) * 3.15f;
      camera.camDir = Math::normalize(fm_vec3_t{0,0,0} - camera.camPos);
    }

    auto fb = &fbs[frame];
    Text::setFrameBuffer(*fb);

    if(redrawMenu != 0) {
      Text::printf(120, 222, "[L/R] SDF:%d", sdfIdx);
      switch (resolution) {
        default:
        case 1: Text::print(230, 222, "[A/B] Full"); break;
        case 2: Text::print(230, 222, "[A/B] 1/2x"); break;
        case 4: Text::print(230, 222, "[A/B] 1/4x"); break;
      }
      --redrawMenu;
    }

    currTime += deltaTime;

    disable_interrupts();

      auto ticks = get_ticks();
      RayMarch::draw(fb->buffer, currTime, sdfIdx, resolution);
      ticks = get_ticks() - ticks;

    enable_interrupts();

    Text::printf(16, 222, "%.2fms``", TICKS_TO_US(ticks) * (1.0f / 1000.0f));

    // Note that we never check if the VI is done with the current buffer
    // this is fine since we are sadly never faster than 60FPS
    vi_show(fb);
  }
}
