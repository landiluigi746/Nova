#include "Nova/Scene/SceneManager.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Core/App.hpp"

#include <GLFW/glfw3.h>

namespace Nova
{
    static SceneManager* s_SceneManager = nullptr;

    SceneManager::SceneManager()
    {
        NOVA_ASSERT(!s_SceneManager, "SceneManager is already initialized");
        s_SceneManager = this;
    }

    void SceneManager::Shutdown()
    {
        m_Scenes.clear();
    }

    void SceneManager::RemoveScene(const std::string_view& name)
    {
        if (auto [found, it] = IsSceneRegistered(name); found)
            m_Scenes.erase(it);
    }

    void SceneManager::StartScene(const std::string_view& name)
    {
        if (auto [found, it] = IsSceneRegistered(name); found && !it->Running && it->Stopped)
        {
            Logger::Info("Starting scene \"{}\"", it->Name);
            it->Scene->Start();
            it->Running = true;
            it->Stopped = false;
        }
    }

    void SceneManager::StopScene(const std::string_view& name)
    {
        if (auto [found, it] = IsSceneRegistered(name); found && it->Running && !it->Stopped)
        {
            Logger::Info("Stopping scene \"{}\"", it->Name);
            it->Scene->End();
            it->Running = false;
            it->Stopped = true;
        }
    }

    void SceneManager::ResumeScene(const std::string_view& name)
    {
        if (auto [found, it] = IsSceneRegistered(name); found && !it->Running && !it->Stopped)
        {
            Logger::Info("Resuming scene \"{}\"", it->Name);
            it->Running = true;
        }
    }

    void SceneManager::PauseScene(const std::string_view& name)
    {
        if (auto [found, it] = IsSceneRegistered(name); found && it->Running && !it->Stopped)
        {
            Logger::Info("Pausing scene \"{}\"", it->Name);
            it->Running = false;
        }
    }

    std::pair<bool, SceneManager::SceneContainer::iterator> SceneManager::IsSceneRegistered(
        const std::string_view& name, bool warn)
    {
        auto it =
            std::find_if(std::begin(m_Scenes), std::end(m_Scenes), [&name](const SceneContainer::value_type& scene) {
                return scene.Name == name;
            });

        bool found = it != std::end(m_Scenes);

        if (!found && warn)
            Logger::Warning("Scene \"{}\" is not registered", name.data());

        return {found, it};
    }

    void SceneManager::ProcessScenes(float deltaTime)
    {
        for (auto& scene : m_Scenes)
        {
            if (!(scene.Running && !scene.Stopped))
                continue;

            scene.Scene->Update(deltaTime);
            scene.Scene->ProcessEasings(deltaTime * 1000.0f);
        }

        for (auto& scene : m_Scenes)
        {
            if (scene.Running && !scene.Stopped)
                scene.Scene->Draw();
        }
    }

    void SceneManager::ProcessImGuiFrame()
    {
        for (auto& scene : m_Scenes)
        {
            if (scene.Running && !scene.Stopped)
                scene.Scene->ImGuiDraw();
        }
    }
} // namespace Nova
