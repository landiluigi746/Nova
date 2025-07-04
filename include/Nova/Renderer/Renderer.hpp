#pragma once

#include "Nova/Misc/Color.hpp"
#include "Nova/Renderer/Texture.hpp"
#include "Nova/Renderer/Sprite.hpp"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace Nova::Renderer
{
    void UpdateProjection(int width, int height);
    void EnableMultisampling();
    void DisableMultisampling();

    void Init(int width, int height);
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    void ClearScreen(const Color& color);

    void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Color& color, float rotation = 0.0f,
                  const glm::vec2& origin = {0.0f, 0.0f});

    void DrawQuad(std::shared_ptr<Texture> texture, const glm::vec2& position, const Color& color = Nova::White,
                  float rotation = 0.0f, const glm::vec2& origin = {0.0f, 0.0f});

    void DrawQuad(std::shared_ptr<Texture> texture, const glm::vec2& position, const glm::vec2& scale,
                  const Color& color = Nova::White, float rotation = 0.0f, const glm::vec2& origin = {0.0f, 0.0f});

    void DrawQuad(std::shared_ptr<Texture> texture, const glm::vec4& sourceRect, const glm::vec2& position,
                  const Color& color, float rotation, const glm::vec2& origin);

    void DrawQuad(std::shared_ptr<Texture> texture, const glm::vec2& position, const glm::vec2& scale,
                  const Color& color, float rotation, const glm::vec2& origin, const glm::vec4& sourceRect);

    void DrawSprite(const Sprite& sprite, const glm::vec2& position, float rotation = 0.0f,
                    const glm::vec2& origin = {0.0f, 0.0f});

    void DrawSprite(const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale, float rotation = 0.0f,
                    const glm::vec2& origin = {0.0f, 0.0f});
} // namespace Nova::Renderer
