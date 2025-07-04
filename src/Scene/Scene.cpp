#include "Nova/Scene/Scene.hpp"
#include "Nova/ECS/Entity.hpp"
#include "Nova/Core/App.hpp"
#include "Nova/Misc/Metrics.hpp"

namespace Nova
{
    Scene::Scene()
        : m_Window(App::Get().GetWindow()), m_SceneManager(App::Get().GetSceneManager()),
          m_AssetManager(App::Get().GetAssetManager()), m_SpriteSystem(this),
          m_RendererSystem(std::make_unique<RendererSystem>(this))
    {
    }

    Entity Scene::CreateEntity()
    {
        Metrics::IncrementEntities();
        return Entity(m_Registry.create(), this);
    }

    void Scene::DestroyEntity(Entity& entity)
    {
        if (entity.m_Entity == entt::null || entity.m_ParentScene != this)
            return;

        m_Registry.destroy(entity.m_Entity);
        entity.m_Entity = entt::null;
        entity.m_ParentScene = nullptr;

        Metrics::DecrementEntities();
    }

    void Scene::AddEasing(const Easing& easing)
    {
        m_Easings.emplace_back(easing);
    }

    void Scene::ProcessEasings(float milliseconds)
    {
        for (auto it = std::begin(m_Easings); it != std::end(m_Easings);)
        {
            it->Update(milliseconds);

            if (it->IsCompleted())
            {
                if (it->m_Config.OnComplete)
                    it->m_Config.OnComplete();

                it = m_Easings.erase(it);
            }
            else
                ++it;
        }
    }

    void Scene::UpdateSystems(float deltaTime)
    {
        m_SpriteSystem.Update(deltaTime);

        for (const auto& system : m_Systems)
            system->Update(deltaTime);
    }
} // namespace Nova
