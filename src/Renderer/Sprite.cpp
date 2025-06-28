#pragma once

#include "Nova/Renderer/Sprite.hpp"

namespace Nova
{
    Sprite::Sprite(const SpriteConfig& config) : m_Config(config) {}

    void Sprite::Update(float deltaTime) noexcept
    {
        m_CurrentTime += deltaTime;

        if (m_CurrentTime < m_Config.AnimationTick)
            return;

        m_CurrentTime -= m_Config.AnimationTick;

        uint32_t maxFrames = m_Config.NumCols * m_Config.NumRows;

        if (m_Config.Loop)
            CurrentFrame = (CurrentFrame + 1) % maxFrames;
        else if (CurrentFrame < maxFrames - 1)
            ++CurrentFrame;

        m_CurrentCol = CurrentFrame % m_Config.NumCols;
        m_CurrentRow = CurrentFrame / m_Config.NumCols;
    }
} // namespace Nova
