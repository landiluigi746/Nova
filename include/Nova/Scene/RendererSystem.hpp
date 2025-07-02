#pragma once

#include "Nova/Scene/System.hpp"

namespace Nova
{
    class RendererSystem : public System
    {
    public:
        using System::System;

        virtual void Update(float deltaTime) override;
    };
} // namespace Nova
