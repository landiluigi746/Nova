#include "Nova/Scene/RendererSystem.hpp"
#include "Nova/Scene/Scene.hpp"
#include "Nova/Scene/Components.hpp"

#include "Nova/Renderer/Renderer.hpp"

namespace Nova
{
    void RendererSystem::Update(float deltaTime)
    {
        // Draw all quads with a color in the scene
        {
            auto view = m_ParentScene->GetEntitiesWith<const QuadTransform, const ColorComponent>();

            view.each([](const QuadTransform& transform, const ColorComponent& color) {
                Renderer::DrawQuad(transform.Position, transform.Size, color, transform.Rotation);
            });
        }

        // Draw all quads with a texture component in the scene
        {
            auto view = m_ParentScene->GetEntitiesWith<const QuadTransform, const TextureComponent>();

            // clang-format off
            view.each([](const QuadTransform& transform, const TextureComponent& texture) {
                Renderer::DrawQuad(texture.Texture, transform.Position, transform.Size, texture.Color, transform.Rotation);
            });
            // clang-format on
        }

        // Draw all quads with a sprite component in the scene
        {
            auto view = m_ParentScene->GetEntitiesWith<const QuadTransform, const SpriteComponent>();

            view.each([](const QuadTransform& transform, const SpriteComponent& sprite) {
                Renderer::DrawSprite(sprite, transform.Position, transform.Size, transform.Rotation);
            });
        }
    }
} // namespace Nova
