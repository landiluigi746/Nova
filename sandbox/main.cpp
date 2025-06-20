#include "Nova/Core/EntryPoint.hpp"
#include "Nova/Scene/Scene.hpp"
#include "Nova/Scene/SceneManager.hpp"
#include "Nova/Core/Input.hpp"
#include "Nova/Renderer/Renderer.hpp"

#include <imgui.h>

class SandboxScene : public Nova::Scene
{
public:
    void Start() override
    {
        Nova::Logger::Info("Hi! Welcome to Nova!");
    }

    void Update(float deltaTime) override
    {
        m_Pos = Nova::Input::GetMousePos();
    }

    void Draw() override
    {
        Nova::Renderer::ClearScreen(Nova::LightGray);
        Nova::Renderer::DrawQuad(m_Pos, m_Size, Nova::White, rotation);
    }

    void ImGuiDraw() override
    {
        ImGui::Begin("Nova Sandbox project");

        ImGui::Text("Welcome to Nova! In this project you can experiment with the framework.");

        ImGui::Separator();

        ImGui::Text("Position: %.2f, %.2f", m_Pos.x, m_Pos.y);
        ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f, "%.2f");
        ImGui::SliderFloat2("Size", &m_Size.x, 0.0f, 100.0f);

        ImGui::End();
    }

private:
    glm::vec2 m_Pos = {0.0f, 0.0f};
    glm::vec2 m_Size = {100.0f, 100.0f};
    float rotation = 0.0f;
};

class SandboxApp : public Nova::App
{
public:
    SandboxApp(const Nova::AppConfig& config) : Nova::App(config)
    {
        Nova::SceneManager& sceneManager = Nova::SceneManager::Get();

        sceneManager.AddScene<SandboxScene>("SandboxScene");
        sceneManager.StartScene("SandboxScene");
    }
};

Nova::AppPtr Nova::CreateApp()
{
    Nova::AppConfig config = {
        .WindowFlags = Nova::WindowFlags_Resizable,
        .Width = 1280,
        .Height = 720,
        .Title = "Nova Sandbox project",
    };

    return Nova::MakeApp<SandboxApp>(config);
}
