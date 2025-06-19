#include "Nova/Core/App.hpp"
#include "Nova/Core/Window.hpp"
#include "Nova/Core/Input.hpp"
#include "Nova/Core/AssetManager.hpp"

#include "Nova/Renderer/Renderer.hpp"

#include "Nova/Scene/SceneManager.hpp"

#include "Nova/Misc/Logger.hpp"
#include "Nova/Misc/Assert.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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

        Window::Init(WindowConfig{
            .Flags = config.WindowFlags, .Width = config.Width, .Height = config.Height, .Title = config.Title});

        Logger::Info("Initializing GLAD...");
        NOVA_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize GLAD!");
        Logger::Info("Initialized GLAD successfully!");

        Logger::Info("OpenGL version: {}", (const char*) glGetString(GL_VERSION));
        Logger::Info("OpenGL vendor: {}", (const char*) glGetString(GL_VENDOR));
        Logger::Info("OpenGL renderer: {}", (const char*) glGetString(GL_RENDERER));

        Renderer::Init();
        InitImGui();
        SceneManager::Init();
        AssetManager::Init();

        Logger::Info("Nova App initialized successfully!");
    }

    void App::Run()
    {
        Window& window = Window::Get();
        SceneManager& sceneManager = SceneManager::Get();

        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!window.ShouldClose())
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            Input::PollEvents();

            Renderer::BeginFrame();
            sceneManager.ProcessScenes(deltaTime);
            Renderer::EndFrame();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            sceneManager.ProcessImGuiFrame();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            window.SwapBuffers();
        }
    }

    void App::Shutdown()
    {
        Logger::Info("Shutting down Nova App...");

        AssetManager::Shutdown();
        SceneManager::Shutdown();
        ShutdownImGui();
        Renderer::Shutdown();
        Window::Shutdown();
        glfwTerminate();

        Logger::Info("Nova App shut down successfully!");
    }

    void App::InitImGui()
    {
        Logger::Info("Initializing ImGui...");

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->AddFontDefault();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(Window::Get().GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");

        Logger::Info("Initialized ImGui successfully!");
    }

    void App::ShutdownImGui()
    {
        Logger::Info("Shutting down ImGui...");

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        Logger::Info("ImGui shut down successfully!");
    }
} // namespace Nova

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui_impl_glfw.cpp>
#include <imgui_impl_opengl3.cpp>
