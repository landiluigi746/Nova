#pragma once

#include "Nova/Misc/Easings.hpp"
#include "Nova/Scene/RendererSystem.hpp"

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

        template<typename SystemT, typename... Args>
        requires(std::is_base_of_v<System, SystemT> && !std::is_base_of_v<RendererSystem, SystemT>)
        void AddSystem(Args&&... args)
        {
            m_Systems.emplace_back(std::make_unique<SystemT>(this, std::forward<Args>(args)...));
        }

        template<typename SystemT, typename... Args>
		requires std::is_base_of_v<RendererSystem, SystemT>
        void SetRendererSystem(Args&&... args)
		{
			m_RendererSystem = std::make_unique<SystemT>(this, std::forward<Args>(args)...);
		}

        Entity CreateEntity();
        void DestroyEntity(Entity& entity);

        template<typename... Components>
        auto GetEntitiesWith()
        {
            return m_Registry.view<Components...>();
        }

        template<typename... Components>
        auto GetEntitiesWith() const
        {
            return m_Registry.view<Components...>();
        }

        entt::registry& GetRegistry()
        {
            return m_Registry;
        }

    protected:
        void AddEasing(const Easing& easing);

    private:
        void UpdateSystems(float deltaTime) const;
        void ProcessEasings(float milliseconds);

    protected:
        Window& m_Window;
        SceneManager& m_SceneManager;
        AssetManager& m_AssetManager;

    private:
        std::unique_ptr<RendererSystem> m_RendererSystem;
        std::vector<std::unique_ptr<System>> m_Systems;
        std::vector<Easing> m_Easings;
        entt::registry m_Registry;

        friend class SceneManager;
        friend class Entity;
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace Nova
