#pragma once

#include <cstdint>

namespace Nova::Metrics
{
    double GetTime();
    float GetDeltaTime();
    float GetFPS();
    uint32_t GetDrawCalls();
    uint32_t GetDrawnObjects();

    void DebugUI();

    float NewFrame();

    void IncrementDrawCalls();
    void IncrementDrawnObjects(uint32_t count);
} // namespace Nova::Metrics
