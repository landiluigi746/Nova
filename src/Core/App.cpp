#include "Nova/Core/App.hpp"
#include "Nova/Core/Window.hpp"
#include "Nova/Core/Input.hpp"

#include "Nova/Asset/AssetManager.hpp"

#include "Nova/Renderer/Renderer.hpp"

#include "Nova/Scene/SceneManager.hpp"

#include "Nova/Misc/Logger.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Misc/Metrics.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <raudio.h>

namespace Nova
{
    static App* s_Instance = nullptr;

    App::App(const AppConfig& config)
    {
        NOVA_ASSERT(!s_Instance, "App already created!");
        s_Instance = this;

        Logger::Info("Initializing Nova App...");

        Logger::Info("Initializing GLFW...");
        NOVA_ASSERT(glfwInit(), "Failed to initialize GLFW");
        Logger::Info("Initialized GLFW successfully!");

        glfwSetErrorCallback([](int code, const char* msg) {
            Nova::Logger::Error("GLFW error: {}, {}", code, msg);
        });

        m_Window.Init(config.Window);

        Logger::Info("Initializing GLAD...");
        NOVA_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize GLAD!");
        Logger::Info("Initialized GLAD successfully!");

        Logger::Info("OpenGL version: {}", (const char*) glGetString(GL_VERSION));
        Logger::Info("OpenGL vendor: {}", (const char*) glGetString(GL_VENDOR));
        Logger::Info("OpenGL renderer: {}", (const char*) glGetString(GL_RENDERER));

        Renderer::Init(m_Window.GetWidth(), m_Window.GetHeight());

        if (config.Window.Flags & WindowFlags_EnableMSAAx4)
            Renderer::EnableMultisampling();

        InitImGui();
		InitAudio();

        Logger::Info("Nova App initialized successfully!");
    }

    App::~App()
    {
        Logger::Info("Shutting down Nova App...");

		ShutdownAudio();
        ShutdownImGui();
        Renderer::Shutdown();
        m_SceneManager.Shutdown();
        m_AssetManager.Shutdown();
        m_Window.Shutdown();
        glfwTerminate();

        Logger::Info("Nova App shut down successfully!");
    }

    App& App::Get()
    {
        NOVA_ASSERT(s_Instance, "App not initialized!");
        return *s_Instance;
    }

    void App::Run()
    {
        while (!m_Window.ShouldClose())
        {
            float deltaTime = Metrics::NewFrame();

            Input::PollEvents();

            Renderer::BeginFrame();
            m_SceneManager.ProcessScenes(deltaTime);
            Renderer::EndFrame();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            m_SceneManager.ProcessImGuiFrame();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            m_Window.SwapBuffers();
        }
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

        ImGui_ImplGlfw_InitForOpenGL(m_Window.GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");

        Logger::Info("ImGui Initialized successfully!");
    }

    void App::ShutdownImGui()
    {
        Logger::Info("Shutting down ImGui...");

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        Logger::Info("ImGui shut down successfully!");
    }

    void App::InitAudio()
    {
        Logger::Info("Initializing audio...");

        InitAudioDevice();
        NOVA_ASSERT(IsAudioDeviceReady(), "Failed to initialize audio device!");

        Logger::Info("Audio initialized successfully!");
    }

    void App::ShutdownAudio()
    {
        Logger::Info("Shutting down audio...");

        CloseAudioDevice();

        Logger::Info("Audio shut down successfully!");
    }
} // namespace Nova

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui_impl_glfw.cpp>
#include <imgui_impl_opengl3.cpp>
