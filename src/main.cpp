#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <random>

#include "Nova/Core/Logger.hpp"
#include "Nova/Core/Assert.hpp"
#include "Nova/Core/Window.hpp"

int main()
{
    NOVA_ASSERT(glfwInit(), "Failed to initialize GLFW!");

    glfwSetErrorCallback([](int code, const char* msg) {
        Nova::Logger::Error("GLFW error: {}, {}", code, msg);
    });

    Nova::Window::Init(Nova::WindowConfig{.Flags = Nova::WindowFlags_Vsync | Nova::WindowFlags_Resizable,
                                          .Width = 1280,
                                          .Height = 720,
                                          .Title = "Nova Test"});

    NOVA_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize GLAD!");

    float r = 0.5f, g = 0.3f, b = 0.8f;

    std::default_random_engine randEngine(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    Nova::Window& window = Nova::Window::Get();

    while (!window.ShouldClose())
    {
        glfwPollEvents();

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // EPILEPSY WARNING!
        /*r = dist(randEngine);
        g = dist(randEngine);
        b = dist(randEngine);*/

        window.SwapBuffers();
    }

    Nova::Window::Shutdown();

    glfwTerminate();

    return 0;
}
