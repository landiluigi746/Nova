#pragma once

namespace Nova
{
    class Scene;

    class System
    {
    public:
        System(Scene* parentScene);
        virtual ~System() = default;

        virtual void Update(float deltaTime) = 0;

    protected:
        Scene* m_ParentScene = nullptr;
    };
} // namespace Nova
