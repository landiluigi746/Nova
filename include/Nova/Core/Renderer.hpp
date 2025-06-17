#pragma once

#include "Nova/Misc/Color.hpp"

#include <glm/vec2.hpp>

namespace Nova::Renderer
{
    void UpdateProjection(int width, int height);

    void Init();
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    void ClearScreen(const Color& color);
    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color);
    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color, float rotation,
                  const glm::vec2& origin);
} // namespace Nova::Renderer
