#pragma once

#include <string>

struct GLFWwindow;

namespace Nova
{
    // clang-format off
    enum WindowFlags : uint8_t
    {
        WindowFlags_None        = 0,
        WindowFlags_Vsync       = 1 << 0,
        WindowFlags_Fullscreen  = 1 << 1,
        WindowFlags_Resizable   = 1 << 2
    };
    // clang-format on

    struct WindowConfig
    {
        uint8_t Flags;
        int Width;
        int Height;
        std::string Title;
    };

    class Window
    {
    public:
        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        static void Init(const WindowConfig& config);
        static void Shutdown();
        static Window& Get();

        bool ShouldClose() const;
        void SwapBuffers() const;

    private:
        Window(const WindowConfig& config);
        ~Window();

    private:
        GLFWwindow* m_Window;
        WindowConfig m_Config;
    };
} // namespace Nova
