#include "Nova/Core/EntryPoint.hpp"
#include "Nova/Core/Scene.hpp"
#include "Nova/Core/SceneManager.hpp"

#include <glad/glad.h>
#include <imgui.h>

class TestScene : public Nova::Scene
{
public:
    void Draw() override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void ImGuiDraw() override
    {
        ImGui::Begin("Test Scene");
        ImGui::Text("Hello, Test Scene!");

        if (ImGui::Button("Pause Another Scene"))
			Nova::SceneManager::Get().PauseScene("AnotherScene");

        if (ImGui::Button("Resume Another Scene"))
			Nova::SceneManager::Get().ResumeScene("AnotherScene");

		if (ImGui::Button("Stop Another Scene"))
			Nova::SceneManager::Get().StopScene("AnotherScene");

		if (ImGui::Button("Start Another Scene"))
			Nova::SceneManager::Get().StartScene("AnotherScene");

        ImGui::End();
    }
};

class AnotherScene : public Nova::Scene
{
public:
    void Start() override
    {
		m_Val = 0;
        Nova::Logger::Info("Another Scene started!");
    }

    void End() override
    {
		Nova::Logger::Info("Another Scene ended!");
    }

    void Draw() override {}

    void ImGuiDraw() override
    {
        ImGui::Begin("Another Scene");
        ImGui::Text("Hello, Another Scene!");
        ImGui::SliderInt("Value", &m_Val, 0, 100);
        ImGui::End();
    }

private:
    int m_Val;
};

class Sandbox : public Nova::App
{
public:
    Sandbox(const Nova::AppConfig& config) : Nova::App(config)
    {
        Nova::SceneManager& sceneManager = Nova::SceneManager::Get();

        sceneManager.AddScene<TestScene>("TestScene");
        sceneManager.StartScene("TestScene");

		sceneManager.AddScene<AnotherScene>("AnotherScene");
        sceneManager.StartScene("AnotherScene");
    }
};

Nova::AppPtr Nova::CreateApp()
{
    Nova::AppConfig config = {.WindowFlags = Nova::WindowFlags_Vsync | Nova::WindowFlags_Resizable,
                              .Width = 1280,
                              .Height = 720,
                              .Title = "Nova Test"};

    return Nova::MakeApp<Sandbox>(config);
}
