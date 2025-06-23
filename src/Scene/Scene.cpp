#include "Nova/Scene/Scene.hpp"

namespace Nova
{
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
} // namespace Nova
