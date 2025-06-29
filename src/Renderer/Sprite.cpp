#pragma once

#include "Nova/Renderer/Sprite.hpp"

namespace Nova
{
    Sprite::Sprite(const SpriteConfig& config) : m_Config(config) {}

    void Sprite::PlayAnimation(const std::string_view& name, bool loop, bool skipIfAlreadyPlaying)
    {
        if (!m_AnimationMap.contains(name))
        {
            Logger::Warning("Animation {} does not exist!", name);
            return;
        }

        if (m_CurrentAnimation == name && skipIfAlreadyPlaying)
            return;

        m_CurrentAnimation = name;
        m_CurrentAnimIndex = 0;
        m_CurrentCol = 0;
        m_CurrentRow = 0;
        m_CurrentTime = 0.0f;
        m_Loop = loop;
    }

    void Sprite::Update(float deltaTime) noexcept
    {
        if (m_CurrentAnimation.empty() || m_AnimationMap[m_CurrentAnimation].empty())
            return;

        m_CurrentTime += deltaTime;

        if (m_CurrentTime < m_Config.AnimationTick)
            return;

        m_CurrentTime -= m_Config.AnimationTick;

        const auto& currentFrames = m_AnimationMap[m_CurrentAnimation];

        if (m_Loop)
            m_CurrentAnimIndex = (m_CurrentAnimIndex + 1) % currentFrames.size();
        else if (m_CurrentAnimIndex < currentFrames.size() - 1)
            m_CurrentAnimIndex++;

        m_CurrentCol = currentFrames[m_CurrentAnimIndex] % m_Config.NumCols;
        m_CurrentRow = currentFrames[m_CurrentAnimIndex] / m_Config.NumCols;
    }
} // namespace Nova
