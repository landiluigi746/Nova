#include "Nova/Core/EntryPoint.hpp"
#include "Nova/Core/Scene.hpp"
#include "Nova/Core/SceneManager.hpp"
#include "Nova/Core/Input.hpp"

#include <glad/glad.h>
#include <imgui.h>

class TestScene : public Nova::Scene
{
public:
    void Update(float deltaTime) override
    {
        if (Nova::Input::IsKeyDown(Nova::Input::Key::W))
            Nova::Logger::Trace("W key down!");

		if (Nova::Input::IsKeyPressed(Nova::Input::Key::A))
			Nova::Logger::Trace("A key pressed!");
    }

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

        glm::vec2 mousePos = Nova::Input::GetMousePos();
		ImGui::Text("Mouse pos: %.2f, %.2f", mousePos.x, mousePos.y);

		glm::vec2 mouseWheel = Nova::Input::GetMouseWheel();
		ImGui::Text("Mouse wheel: %f, %f", mouseWheel.x, mouseWheel.y);

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
        Nova::Logger::SetMinimumLogLevel(Nova::Logger::Level::Trace);

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
