#pragma once

#include "Nova/ECS/System.hpp"

namespace Nova
{
    class RendererSystem : public System
    {
    public:
        using System::System;

        virtual void Update(float deltaTime) override;
    };
} // namespace Nova
