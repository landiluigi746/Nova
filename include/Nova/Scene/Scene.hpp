#pragma once

#include "Nova/Misc/Easings.hpp"

#include <vector>
#include <memory>

namespace Nova
{
    class Scene
    {
    public:
        Scene() = default;

        virtual ~Scene() {}

        virtual void Start() {}
        virtual void End() {}
        virtual void Update(float deltaTime) {}
        virtual void Draw() = 0;
        virtual void ImGuiDraw() {}

    protected:
        void AddEasing(const Easing& easing);

    private:
        void ProcessEasings(float milliseconds);

    private:
        std::vector<Easing> m_Easings;

        friend class SceneManager;
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace Nova
