/**
* @copyright 2024 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>
#include <vector>
#include <stdexcept>

namespace RDP
{
  struct DPL
  {
    uint64_t *dpl;
    uint64_t *dplEnd;
    uint64_t *dplCapEnd;

    DPL(uint32_t cmdCount = 100) {
      dpl = (uint64_t*)malloc_uncached(sizeof(uint64_t) * cmdCount);
      dplEnd = dpl;
      dplCapEnd = dpl + cmdCount;
    }

    ~DPL() {
      free_uncached(dpl);
    }

    void reset() {
      dplEnd = dpl;
    }

    // move constructor to avoid freeing memory twice
    DPL(DPL&& other) {
      dpl = other.dpl;
      dplEnd = other.dplEnd;
      other.dpl = nullptr;
      other.dplEnd = nullptr;
    }

    DPL& add(uint64_t cmd) {
      *dplEnd = cmd;
      dplEnd++;
      assertf(dplEnd <= dplCapEnd, "DPL overflow: %d/%d", (int)(dplEnd - dpl), (int)(dplCapEnd - dpl));
      return *this;
    }

    DPL& add(const std::vector<uint64_t> &cmds) {
      for (auto cmd : cmds)add(cmd);
      return *this;
    }

    void runAsync() const {
      while (*DP_STATUS & DP_STATUS_DMA_BUSY) {};
      MEMORY_BARRIER();
      *DP_START = PhysicalAddr(dpl);
      MEMORY_BARRIER();
      *DP_END = PhysicalAddr(dplEnd);
      MEMORY_BARRIER();
    }

    void runSync() {
      add(RDP::syncFull());
      runAsync();
      MEMORY_BARRIER();
      while (*DP_STATUS & DP_STATUS_PIPE_BUSY) {};
    }
  };
}