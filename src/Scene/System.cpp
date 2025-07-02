#include "Nova/Scene/System.hpp"
#include "Nova/Misc/Assert.hpp"

namespace Nova
{
    System::System(Scene* parentScene)
    {
        NOVA_ASSERT(parentScene, "A system's parent scene cannot be null");

        m_ParentScene = parentScene;
    }
} // namespace Nova
