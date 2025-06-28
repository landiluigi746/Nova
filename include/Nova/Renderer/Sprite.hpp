#pragma once

#include "Nova/Asset/Assets.hpp"

#include <glm/vec2.hpp>

namespace Nova
{
    struct SpriteConfig
    {
        TextureAsset Texture;
        uint32_t FrameWidth = 0;
        uint32_t FrameHeight = 0;
        uint32_t NumCols = 1;
        uint32_t NumRows = 1;
        float AnimationTick = 1.0f;
        bool Loop = false;
    };

    class Sprite
    {
    public:
        Sprite() = default;
        Sprite(const SpriteConfig& config);

        void Update(float deltaTime) noexcept;

        const TextureAsset& GetTexture() const noexcept
        {
            return m_Config.Texture;
        }

        uint32_t GetCurrentRow() const noexcept
        {
            return m_CurrentRow;
        }

        glm::vec2 GetFrameSize() const noexcept
        {
            return {(float) m_Config.FrameWidth, (float) m_Config.FrameHeight};
        }

        glm::vec2 GetFramePosition() const noexcept
        {
            return {(float) m_CurrentCol * (float) m_Config.FrameWidth,
                    (float) m_CurrentRow * (float) m_Config.FrameHeight};
        }

    public:
        bool FlipX = false;
        uint32_t CurrentFrame = 0;

    private:
        uint32_t m_CurrentCol = 0;
        uint32_t m_CurrentRow = 0;
        float m_CurrentTime = 0.0f;
        SpriteConfig m_Config;
    };
} // namespace Nova
