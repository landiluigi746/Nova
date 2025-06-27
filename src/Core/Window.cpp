#include "Nova/Core/Window.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Core/Input.hpp"
#include "Nova/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>

namespace Nova
{
    static Window* s_Window = nullptr;

    Window::Window()
    {
        NOVA_ASSERT(!s_Window, "Window already created!");
        s_Window = this;
    }

    void Window::Init(const WindowConfig& config)
    {
        NOVA_ASSERT(config.Width > 0 && config.Height > 0, "Invalid window dimensions");
        NOVA_ASSERT(!config.Title.empty(), "Window title cannot be empty");

        m_Config = config;

        Logger::Info("Creating window...");

        glfwWindowHint(GLFW_RESIZABLE, (m_Config.Flags & WindowFlags_Resizable));
        glfwWindowHint(GLFW_DECORATED, (m_Config.Flags & WindowFlags_Undecorated) == 0);

        if (m_Config.Flags & WindowFlags_EnableMSAAx4)
            glfwWindowHint(GLFW_SAMPLES, 4);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWmonitor* monitor = nullptr;

        if (m_Config.Flags & WindowFlags_Fullscreen)
        {
            monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            m_Config.Width = mode->width;
            m_Config.Height = mode->height;
        }

        m_Window = glfwCreateWindow(m_Config.Width, m_Config.Height, m_Config.Title.c_str(), monitor, nullptr);

        NOVA_ASSERT_CLEANUP_FUNC(m_Window, "Failed to create window!", [] {
            glfwTerminate();
        });

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, this);

        glfwSetKeyCallback(m_Window, &Input::KeyCallback);
        glfwSetMouseButtonCallback(m_Window, &Input::MouseButtonCallback);
        glfwSetScrollCallback(m_Window, &Input::MouseScrollCallback);

        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* glfwWindow, int width, int height) {
            Window& window = *(Window*) glfwGetWindowUserPointer(glfwWindow);

            window.m_Config.Width = width;
            window.m_Config.Height = height;

            if (window.m_Config.Flags & WindowFlags_Vsync)
                glfwSwapInterval(1);

            Renderer::UpdateProjection(width, height);
        });

        if (m_Config.Flags & WindowFlags_Vsync)
        {
            Logger::Info("Enabling vsync...");
            glfwSwapInterval(1);
        }

        Logger::Info("Window created and initialized successfully!");
    }

    void Window::Shutdown()
    {
        Logger::Info("Destroying window...");
        glfwDestroyWindow(m_Window);
        Logger::Info("Window destroyed successfully!");
    }

    void Window::Resize(int width, int height) const
    {
        if ((m_Config.Flags & WindowFlags_Resizable) == 0)
        {
            Logger::Warning("Cannot resize a non-resizable window!");
            return;
        }

        glfwSetWindowSize(m_Window, width, height);
    }

    void Window::Close() const
    {
        glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
    }

    void Window::Minimize() const
    {
        glfwIconifyWindow(m_Window);
    }

    void Window::Maximize() const
    {
        if ((m_Config.Flags & WindowFlags_Resizable) == 0)
        {
            Logger::Warning("Cannot maximize a non-resizable window!");
            return;
        }

        glfwMaximizeWindow(m_Window);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::SwapBuffers() const
    {
        glfwSwapBuffers(m_Window);
    }
} // namespace Nova
