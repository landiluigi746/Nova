#pragma once

#include "Nova/Asset/Assets.hpp"
#include "Nova/Misc/Logger.hpp"

#include <string>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <vector>
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
    };

    class Sprite
    {
    public:
        Sprite() = default;
        Sprite(const SpriteConfig& config);

        template<size_t N>
        void AddAnimation(const std::string& name, const std::array<uint32_t, N>& frames)
        {
            if (N > m_Config.NumCols * m_Config.NumRows)
            {
                Logger::Warning("Animation {} has more frames than the sprite has space for", name);
                return;
            }

            for (auto frame : frames)
            {
                if (frame >= m_Config.NumCols * m_Config.NumRows)
                {
                    Logger::Warning("Animation {} has frame {} which is out of range", name, frame);
                    return;
                }
            }

            m_AnimationMap[name] = std::vector(std::begin(frames), std::end(frames));
        }

        void PlayAnimation(const std::string& name, bool loop = true, bool skipIfAlreadyPlaying = true);

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

    private:
        bool m_Loop = true;
        uint32_t m_CurrentCol = 0;
        uint32_t m_CurrentRow = 0;
        uint32_t m_CurrentAnimIndex = 0;
        float m_CurrentTime = 0.0f;
        SpriteConfig m_Config;
        std::string m_CurrentAnimation;
        std::unordered_map<std::string, std::vector<uint32_t>> m_AnimationMap;
    };
} // namespace Nova
