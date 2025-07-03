#pragma once

#include "Nova/Scene/Scene.hpp"
#include "Nova/Misc/Logger.hpp"

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <string_view>

namespace Nova
{
    class AssetManager;

    class SceneManager
    {
    public:
        SceneManager();

        SceneManager(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager& operator=(SceneManager&&) = delete;

        void Shutdown();

        void ProcessScenes(float deltaTime);
        void ProcessImGuiFrame();

        template<typename SceneT, typename... Args>
            requires std::is_base_of_v<Scene, SceneT>
        void AddScene(std::string name, Args&&... args)
        {
            if (auto [found, it] = IsSceneRegistered(name, false); found)
            {
                Logger::Warning("The scene \"{}\" is already registered", name);
                return;
            }

            Logger::Info("Registering scene \"{}\"", name);
            m_Scenes.emplace_back(SceneData{.Running = false,
                                            .Stopped = true,
                                            .Name = std::move(name),
                                            .Scene = std::make_shared<SceneT>(std::forward<Args>(args)...)});
        }

        void RemoveScene(const std::string_view& name);

        void StartScene(const std::string_view& name);
        void StopScene(const std::string_view& name);

        void ResumeScene(const std::string_view& name);
        void PauseScene(const std::string_view& name);

    private:
        struct SceneData
        {
            bool Running;
            bool Stopped;
            std::string Name;
            ScenePtr Scene;
        };

        using SceneContainer = std::vector<SceneData>;

        std::pair<bool, SceneContainer::iterator> IsSceneRegistered(const std::string_view& name, bool warn = true);

    private:
        SceneContainer m_Scenes;
    };
} // namespace Nova
