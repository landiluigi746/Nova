#pragma once

#include "Nova/ECS/System.hpp"

namespace Nova
{
    class SpriteSystem : public System
    {
    public:
        using System::System;

        virtual ~SpriteSystem() = default;
        virtual void Update(float deltaTime) override;
    };
} // namespace Nova
