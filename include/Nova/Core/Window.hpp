#pragma once

#include "Nova/Core/WindowConfig.hpp"

#include <string>
#include <string_view>
#include <cstdint>

struct GLFWwindow;

namespace Nova
{
    class Window
    {
    public:
        Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        void Init(const WindowConfig& config);
        void Shutdown();
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
        GLFWwindow* m_Window;
        WindowConfig m_Config;
    };
} // namespace Nova
