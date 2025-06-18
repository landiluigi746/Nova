#include "Nova/Core/EntryPoint.hpp"
#include "Nova/Scene/Scene.hpp"
#include "Nova/Scene/SceneManager.hpp"
#include "Nova/Core/Input.hpp"
#include "Nova/Renderer/Renderer.hpp"

#include <glad/glad.h>
#include <imgui.h>

class TestScene : public Nova::Scene
{
public:
    TestScene()
    {
        // decomment to load texture
        // m_Texture.Init("image.png"); 
    }

    void Update(float deltaTime) override
    {
        m_Pos = Nova::Input::GetMousePos();
    }

    void Draw() override
    {
        Nova::Renderer::ClearScreen({51, 76, 76});
        //Nova::Renderer::DrawQuad(m_Pos, m_Size, Nova::White, rotation);
        Nova::Renderer::DrawQuad(m_Texture, m_Pos, m_Size, Nova::White, rotation);
    }

    void ImGuiDraw() override
    {
        Nova::Window& window = Nova::Window::Get();

        ImGui::Begin("Test Scene");

        ImGui::Text("Position: %.2f, %.2f", m_Pos.x, m_Pos.y);
		ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f, "%.2f");
		ImGui::SliderFloat2("Size", &m_Size.x, 0.0f, 100.0f);

        ImGui::End();
    }

private:
    glm::vec2 m_Pos = {0.0f, 0.0f};
    glm::vec2 m_Size = {100.0f, 100.0f};
    float rotation = 0.0f;

    Nova::Texture m_Texture;
};

class Sandbox : public Nova::App
{
public:
    Sandbox(const Nova::AppConfig& config) : Nova::App(config)
    {
        Nova::SceneManager& sceneManager = Nova::SceneManager::Get();

        sceneManager.AddScene<TestScene>("TestScene");
        sceneManager.StartScene("TestScene");
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
