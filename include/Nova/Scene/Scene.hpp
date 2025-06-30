#pragma once

#include "Nova/Misc/Easings.hpp"

#include <vector>
#include <memory>
#include <entt/entity/registry.hpp>

namespace Nova
{
    class Window;
    class SceneManager;
    class AssetManager;

    class Entity;

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
        Entity CreateEntity();
        void DestroyEntity(Entity& entity);

        template<typename... Components>
        auto GetEntitiesWith() const
        {
            return m_Registry.view<Components...>();
        }

        void AddEasing(const Easing& easing);

    private:
        void ProcessEasings(float milliseconds);

    protected:
        Window& m_Window;
        SceneManager& m_SceneManager;
        AssetManager& m_AssetManager;

    private:
        std::vector<Easing> m_Easings;
        entt::registry m_Registry;

        friend class SceneManager;
        friend class Entity;
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace Nova
