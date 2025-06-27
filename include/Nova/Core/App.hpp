#pragma once

#include "Nova/Core/Window.hpp"
#include "Nova/Asset/AssetManager.hpp"
#include "Nova/Scene/SceneManager.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <type_traits>

int main();

namespace Nova
{
    struct AppConfig
    {
        WindowConfig Window;
    };

    class App
    {
    public:
        App(const AppConfig& config);
        virtual ~App();

        static App& Get();

        // clang-format off
        Window& GetWindow() { return m_Window; }
		AssetManager& GetAssetManager() { return m_AssetManager; }
		SceneManager& GetSceneManager() { return m_SceneManager; }
        // clang-format on

    protected:
        Window m_Window;
        AssetManager m_AssetManager;
        SceneManager m_SceneManager;

    private:
        void Run();

        void InitImGui();
        void ShutdownImGui();

        friend int ::main();
    };

    using AppPtr = std::unique_ptr<App>;

    template<typename AppT, typename... Args>
    inline AppPtr MakeApp(Args&&... args)
    {
        static_assert(std::is_base_of_v<App, AppT>, "App must derive from Nova::Application!");
        return std::make_unique<AppT>(std::forward<Args>(args)...);
    }

    AppPtr CreateApp();
} // namespace Nova