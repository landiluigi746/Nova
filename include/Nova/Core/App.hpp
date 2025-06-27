#pragma once

#include "Nova/Core/WindowConfig.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <type_traits>

int main();

namespace Nova
{
    class SceneManager;

    struct AppConfig
    {
        WindowConfig Window;
    };

    class App
    {
    public:
        App(const AppConfig& config);
        virtual ~App();

    protected:
    private:
        void Init(const AppConfig& config);
        void Run();
        void Shutdown();

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