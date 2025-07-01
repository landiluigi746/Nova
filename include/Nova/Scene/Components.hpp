#pragma once

#include "Nova/Misc/Color.hpp"
#include "Nova/Renderer/Texture.hpp"
#include "Nova/Renderer/Sprite.hpp"

#include <glm/vec2.hpp>

namespace Nova
{
    struct QuadTransform
    {
        glm::vec2 Position;
        glm::vec2 Size;
        float Rotation;
    };

    using ColorComponent = Color;

    struct TextureComponent
    {
        TextureAsset Texture;
        ColorComponent Color = Nova::White;
    };

    struct SpriteComponent
    {
        Sprite Sprite;
        ColorComponent Color = Nova::White;
    };
} // namespace Nova
