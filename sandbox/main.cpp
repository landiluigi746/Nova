// Make sure to include EntryPoint.hpp since it defines the actual entry point of the application
#include <Nova/Core/EntryPoint.hpp>

// Other headers used in this sandbox project
#include <Nova/Misc/Logger.hpp>
#include <Nova/Scene/Scene.hpp>
#include <Nova/Scene/SceneManager.hpp>
#include <Nova/Core/Input.hpp>
#include <Nova/Renderer/Renderer.hpp>

#include <imgui.h>

// This class defines a simple scene for the sandbox project.
// It demonstrates basic usage of the Nova framework: input handling, rendering, and ImGui UI.
// You can experiment with the code to see how it works.
// Currently, the scene just draws a white square that follows the mouse and its size/rotation 
// can be controlled with ImGui.
class SandboxScene : public Nova::Scene
{
public:
    // This function gets called when you do 
    // something like sceneManager.StartScene("SandboxScene");
    void Start() override
    {
        Nova::Logger::Info("Hi! Welcome to Nova!");
    }

    // This function gets called every frame
    void Update(float deltaTime) override
    {
        m_Pos = Nova::Input::GetMousePos();
    }

    // This function gets called every frame after Update
    void Draw() override
    {
        Nova::Renderer::ClearScreen(Nova::Gray);
        Nova::Renderer::DrawQuad(m_Pos, m_Size, Nova::White, rotation);
    }

    // This function gets called every frame after Draw (so ImGui is drawn on top of the scene)
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
    // Here we define some variables for the scene
    glm::vec2 m_Pos = {0.0f, 0.0f};
    glm::vec2 m_Size = {100.0f, 100.0f};
    float rotation = 0.0f;
};

// Main application class for the sandbox project.
// Sets up and starts the SandboxScene.
class SandboxApp : public Nova::App
{
public:
    SandboxApp(const Nova::AppConfig& config) : Nova::App(config)
    {
        // Get the instance of the SceneManager
        Nova::SceneManager& sceneManager = Nova::SceneManager::Get();

        // Add and start the SandboxScene
        sceneManager.AddScene<SandboxScene>("SandboxScene");
        sceneManager.StartScene("SandboxScene");
    }
};

// Nova framework entry point.
// Creates and returns the main application instance with the desired app configuration.
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
