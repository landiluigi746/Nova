#include "Nova/ECS/Entity.hpp"
#include "Nova/Misc/Assert.hpp"

namespace Nova
{
    Entity::Entity(entt::entity entity, Scene* parentScene)
    {
        NOVA_ASSERT(parentScene, "An entity's parent scene cannot be null");

        m_ParentScene = parentScene;
        m_Entity = entity;
    }

    Entity Entity::Duplicate() const
    {
        Entity other;

        if (!m_ParentScene)
			return other;

        other.m_ParentScene = m_ParentScene;
        other.m_Entity = m_ParentScene->m_Registry.create();

        for (auto [id, storage] : m_ParentScene->m_Registry.storage())
        {
            if (storage.contains(m_Entity))
                storage.push(other.m_Entity, storage.value(m_Entity));
        }

        return other;
    }
} // namespace Nova
