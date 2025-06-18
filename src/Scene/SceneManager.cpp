#include "Nova/Scene/SceneManager.hpp"
#include "Nova/Misc/Assert.hpp"

#include <GLFW/glfw3.h>

namespace Nova
{
    static SceneManager* s_SceneManager = nullptr;

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
        const std::string_view& name)
    {
        auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&name](const SceneContainer::value_type& scene) {
            return scene.Name == name;
        });

        bool found = it != m_Scenes.end();

        if (!found)
            Logger::Warning("Scene \"{}\" is not registered", name.data());

        return {found, it};
    }

    void SceneManager::ProcessScenes(float deltaTime)
    {
		for (auto& scene : m_Scenes)
		{
			if (scene.Running && !scene.Stopped)
				scene.Scene->Update(deltaTime);
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

    void SceneManager::Init()
    {
        if (s_SceneManager)
            return;

        Logger::Info("Initializing SceneManager...");
        s_SceneManager = new SceneManager();
		Logger::Info("SceneManager initialized successfully!");
    }

    void SceneManager::Shutdown()
    {
        if (!s_SceneManager)
            return;

		Logger::Info("Shutting down SceneManager...");
        delete s_SceneManager;
        s_SceneManager = nullptr;
		Logger::Info("SceneManager shut down successfully!");
    }

    SceneManager& SceneManager::Get()
    {
        NOVA_ASSERT_CLEANUP_FUNC(s_SceneManager, "SceneManager is not initialized", [] {
            glfwTerminate();
        });

        return *s_SceneManager;
    }
} // namespace Nova
