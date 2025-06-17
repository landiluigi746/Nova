#include "Nova/Core/App.hpp"
#include "Nova/Core/Window.hpp"
#include "Nova/Core/Logger.hpp"
#include "Nova/Core/Assert.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

namespace Nova
{
    App::App(const AppConfig& config)
    {
        Init(config);
    }

    App::~App()
    {
        Shutdown();
    }

    void App::Init(const AppConfig& config)
    {
        Logger::Info("Initializing Nova App...");

        Logger::Info("Initializing GLFW...");
        NOVA_ASSERT(glfwInit(), "Failed to initialize GLFW");
        Logger::Info("Initialized GLFW successfully!");

        glfwSetErrorCallback([](int code, const char* msg) {
            Nova::Logger::Error("GLFW error: {}, {}", code, msg);
        });

        Window::Init(WindowConfig{.Flags = config.WindowFlags,
                                  .Width = config.Width,
                                  .Height = config.Height,
                                  .Title = config.Title});

        NOVA_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize GLAD!");

		Logger::Info("Nova App initialized successfully!");
    }

    void App::Run()
    {
        Window& window = Window::Get();

        while (!window.ShouldClose())
        {
            glfwPollEvents();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

            window.SwapBuffers();
        }
    }

    void App::Shutdown()
    {
		Logger::Info("Shutting down Nova App...");

        Window::Shutdown();
        glfwTerminate();

		Logger::Info("Nova App shut down successfully!");
    }
} // namespace Nova
