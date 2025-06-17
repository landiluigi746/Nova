#include "Nova/Core/EntryPoint.hpp"

class Sandbox : public Nova::App
{
public:
    Sandbox(const Nova::AppConfig& config) : Nova::App(config) {}
};

Nova::AppPtr Nova::CreateApp()
{
    Nova::AppConfig config = {.WindowFlags = Nova::WindowFlags_Vsync | Nova::WindowFlags_Resizable,
                              .Width = 1280,
                              .Height = 720,
                              .Title = "Nova Test"};

    return Nova::MakeApp<Sandbox>(config);
}
