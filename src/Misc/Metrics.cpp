#include "Nova/Misc/Metrics.hpp"

#include <chrono>
#include <GLFW/glfw3.h>
#include <imgui.h>

namespace Nova::Metrics
{
    struct MetricsData
    {
        float DeltaTime = 0.0f;
        float FPS = 0.0f;
        uint32_t DrawCalls = 0;
        uint32_t DrawnObjects = 0;
        uint32_t Entities = 0;
    };

    static MetricsData s_Data;

    float NewFrame()
    {
        static auto lastTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();

        s_Data.DeltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        s_Data.FPS = 1.0f / s_Data.DeltaTime;
        s_Data.DrawCalls = 0;
        s_Data.DrawnObjects = 0;

        return s_Data.DeltaTime;
    }

    void IncrementDrawCalls()
    {
        ++s_Data.DrawCalls;
    }

    void IncrementDrawnObjects(uint32_t count)
    {
        s_Data.DrawnObjects += count;
    }

    void IncrementEntities()
    {
        ++s_Data.Entities;
    }

    void DecrementEntities()
    {
        --s_Data.Entities;
    }

    void DebugUI()
    {
#if defined(NOVA_DEBUG)
        ImGui::Begin("Nova Debug Metrics");

        ImGui::Value("Delta Time", s_Data.DeltaTime, "%f");
        ImGui::Value("FPS", s_Data.FPS, "%.2f");
        ImGui::Value("Draw Calls", s_Data.DrawCalls);
        ImGui::Value("Drawn Objects", s_Data.DrawnObjects);
        ImGui::Value("Entities", s_Data.Entities);

        ImGui::End();
#endif
    }

    double GetTime()
    {
        return glfwGetTime();
    }

    float GetDeltaTime()
    {
        return s_Data.DeltaTime;
    }

    float GetFPS()
    {
        return s_Data.FPS;
    }

    uint32_t GetDrawCalls()
    {
        return s_Data.DrawCalls;
    }

    uint32_t GetDrawnObjects()
    {
        return s_Data.DrawnObjects;
    }
} // namespace Nova::Metrics
