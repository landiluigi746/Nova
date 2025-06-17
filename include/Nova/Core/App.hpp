#pragma once

#include "Nova/Core/Window.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <type_traits>

int main();

namespace Nova
{
    struct AppConfig
    {
        uint8_t WindowFlags = WindowFlags_None;
        int Width;
        int Height;
        std::string Title = "Nova App";
    };

    class App
    {
    public:
        App(const AppConfig& config);
        ~App();

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