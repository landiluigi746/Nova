#include "Nova/Core/EntryPoint.hpp"
#include "Nova/Scene/Scene.hpp"
#include "Nova/Scene/SceneManager.hpp"
#include "Nova/Core/Input.hpp"
#include "Nova/Renderer/Renderer.hpp"
#include "Nova/Renderer/Texture.hpp"
#include "Nova/Renderer/Shader.hpp"
#include "Nova/Core/AssetManager.hpp"

#include <glad/glad.h>
#include <imgui.h>

class TestScene : public Nova::Scene
{
public:
    TestScene()
    {
        m_Texture = Nova::AssetManager::GetTexture("player");
    }

    void Update(float deltaTime) override
    {
        m_Pos = Nova::Input::GetMousePos();
    }

    void Draw() override
    {
        Nova::Renderer::ClearScreen({51, 76, 76});

        for (int i = 0; i < 10; i++)
        {
            Nova::Renderer::DrawQuad(m_Pos + glm::vec2(i * m_Size.x, i * m_Size.y), m_Size, Nova::White, rotation);
        }

        Nova::Renderer::DrawQuad(m_Texture, m_Pos - m_Size, m_Size, Nova::White, rotation);
    }

    void ImGuiDraw() override
    {
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

    Nova::TextureAsset m_Texture;
};

class Sandbox : public Nova::App
{
public:
    Sandbox(const Nova::AppConfig& config) : Nova::App(config)
    {
        Nova::AssetManager::LoadTexture("player", "negro2.png");
        Nova::AssetManager::LoadFromDirectory("res");

        Nova::SceneManager& sceneManager = Nova::SceneManager::Get();

        sceneManager.AddScene<TestScene>("TestScene");
        sceneManager.StartScene("TestScene");

        Nova::Texture s;
    }
};

Nova::AppPtr Nova::CreateApp()
{
    Nova::AppConfig config = {
        .WindowFlags = Nova::WindowFlags_Resizable, .Width = 1280, .Height = 720, .Title = "Nova Test"};

    return Nova::MakeApp<Sandbox>(config);
}
