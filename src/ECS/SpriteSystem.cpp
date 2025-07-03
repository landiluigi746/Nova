#include "Nova/ECS/SpriteSystem.hpp"
#include "Nova/ECS/Components.hpp"

#include "Nova/Scene/Scene.hpp"

namespace Nova
{
    void SpriteSystem::Update(float deltaTime)
    {
        auto view = m_ParentScene->GetEntitiesWith<SpriteComponent>();

        view.each([deltaTime](SpriteComponent& sprite) {
            sprite.Update(deltaTime);
        });
    }
} // namespace Nova
