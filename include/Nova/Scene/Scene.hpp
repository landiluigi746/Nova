#pragma once

#include "Nova/Misc/Easings.hpp"

#include <vector>
#include <memory>

namespace Nova
{
    class Window;
    class SceneManager;
    class AssetManager;

    class Scene
    {
    public:
        Scene();

        virtual ~Scene() = default;

        virtual void Start() {}
        virtual void End() {}
        virtual void Update(float deltaTime) {}
        virtual void Draw() = 0;
        virtual void ImGuiDraw() {}

    protected:
        void AddEasing(const Easing& easing);

    private:
        void ProcessEasings(float milliseconds);

    protected:
        Window& m_Window;
        SceneManager& m_SceneManager;
        AssetManager& m_AssetManager;

    private:
        std::vector<Easing> m_Easings;

        friend class SceneManager;
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace Nova
