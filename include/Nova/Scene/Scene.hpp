#pragma once

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
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace Nova
