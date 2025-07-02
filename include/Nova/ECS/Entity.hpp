#pragma once

#include "Nova/Scene/Scene.hpp"

#include <entt/entity/entity.hpp>

namespace Nova
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity entity, Scene* scene);

        Entity Duplicate() const;

        template<typename Component>
        bool HasComponent() const
        {
            return m_ParentScene->m_Registry.all_of<Component>(m_Entity);
        }

        template<typename Component, typename... Args>
        Component& AddComponent(Args&&... args)
        {
            return m_ParentScene->m_Registry.emplace<Component>(m_Entity, std::forward<Args>(args)...);
        }

        template<typename Component>
        Component& GetComponent()
        {
            return m_ParentScene->m_Registry.get<Component>(m_Entity);
        }

        template<typename Component>
        const Component& GetComponent() const
        {
            return m_ParentScene->m_Registry.get<Component>(m_Entity);
        }

        template<typename Component>
        void RemoveComponent()
        {
            m_ParentScene->m_Registry.remove<Component>(m_Entity);
        }

        operator entt::entity() const noexcept
		{
			return m_Entity;
		}

        operator bool() const noexcept
        {
            return m_Entity != entt::null && m_ParentScene;
        }

        bool operator==(const Entity& other) const noexcept = default;

    private:
        entt::entity m_Entity = entt::null;
        Scene* m_ParentScene = nullptr;

        friend class Scene;
    };
} // namespace Nova
