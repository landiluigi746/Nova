#pragma once

#include <string>
#include <string_view>
#include <cstdint>

struct GLFWwindow;

namespace Nova
{
    // clang-format off
    enum WindowFlags : uint16_t
    {
        WindowFlags_None        = 0,
        WindowFlags_Vsync       = 1 << 0,
        WindowFlags_Fullscreen  = 1 << 1,
        WindowFlags_Resizable   = 1 << 2,
        WindowFlags_Undecorated = 1 << 3
    };
    // clang-format on

    struct WindowConfig
    {
        uint16_t Flags;
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

        void Resize(int width, int height) const;
        void Close() const;
        void Minimize() const;
        void Maximize() const;
        bool ShouldClose() const;
        void SwapBuffers() const;

        int GetWidth() const noexcept
        {
            return m_Config.Width;
        }

        int GetHeight() const noexcept
        {
            return m_Config.Height;
        }

        std::string_view GetTitle() const noexcept
        {
            return m_Config.Title;
        }

        GLFWwindow* GetNativeWindow() const noexcept
        {
            return m_Window;
        }

    private:
        Window(const WindowConfig& config);
        ~Window();

    private:
        GLFWwindow* m_Window;
        WindowConfig m_Config;
    };
} // namespace Nova
