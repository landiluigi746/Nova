#include "Nova/Core/Window.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Core/Input.hpp"
#include "Nova/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>

namespace Nova
{
    static Window* s_Window = nullptr;

    Window::Window(const WindowConfig& config) : m_Config(config)
    {
        NOVA_ASSERT(config.Width > 0 && config.Height > 0, "Invalid window dimensions");
        NOVA_ASSERT(!config.Title.empty(), "Window title cannot be empty");

        Logger::Info("Creating window...");

        glfwWindowHint(GLFW_RESIZABLE, (m_Config.Flags & WindowFlags_Resizable));
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWmonitor* monitor = (m_Config.Flags & WindowFlags_Fullscreen) ? glfwGetPrimaryMonitor() : nullptr;

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

    Window::~Window()
    {
        Logger::Info("Destroying window...");
        glfwDestroyWindow(m_Window);
        Logger::Info("Window destroyed successfully!");
    }

    void Window::Close() const
    {
        glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::SwapBuffers() const
    {
        glfwSwapBuffers(m_Window);
    }

    void Window::Init(const WindowConfig& config)
    {
        if (s_Window)
            return;

        s_Window = new Window(config);
    }

    void Window::Shutdown()
    {
        if (!s_Window)
            return;

        delete s_Window;
        s_Window = nullptr;
    }

    Window& Window::Get()
    {
        NOVA_ASSERT_CLEANUP_FUNC(s_Window, "Window is not initialized", [] {
            glfwTerminate();
        });

        return *s_Window;
    }
} // namespace Nova
