// Make sure to include EntryPoint.hpp since it defines the actual entry point of the application
#include <Nova/Core/EntryPoint.hpp>

/*.
    Nova Sandbox project

    This project is a simple sandbox project that demonstrates the basic usage of the Nova framework.
    You can experiment with the code to see how it works.

    Currently, the project involves:
    - A simple main application class (SandboxApp) that sets up and starts the main scene
    - A simple scene class (SandboxScene) that:
        - When started, it creates some entities with the ECS, which are later processed by a system added to the scene
        - When updated, it moves a white square that follows the mouse
        - When rendering happens, it clears the screen and then renders the white square
        - When ImGui is drawn on top of the scene, it allows you to control the square's size/rotation
    - A simple movement system (MovementSystem) that moves entities based on their direction, making them bounce between
      the top and bottom of the screen

    Note: ECS entities are drawn automatically in the scene by the default RendererSystem
*/

// Other headers used in this sandbox project
#include <Nova/Misc/Logger.hpp>
#include <Nova/Misc/Metrics.hpp>
#include <Nova/Scene/Scene.hpp>
#include <Nova/Scene/SceneManager.hpp>
#include <Nova/ECS/Entity.hpp>
#include <Nova/ECS/Components.hpp>
#include <Nova/Core/Input.hpp>
#include <Nova/Core/Window.hpp>
#include <Nova/Core/App.hpp>
#include <Nova/Renderer/Renderer.hpp>

#include <imgui.h>

// Direction of movement, can be either 1.0f or -1.0f (up or down)
using Direction = float;

// This class defines a simple movement system that moves entities based on their direction
// making them bounce between the top and bottom of the screen
class MovementSystem : public Nova::System
{
public:
    // This is the base speed of the movement
    static constexpr float SPEED = 250.0f;

    // Inherit the constructor from the base class
    // This allows us to use the base class constructor
    // so you don't need to write it yourself
    using Nova::System::System;

    // This function gets called every frame
    // Here we move the entities based on their direction
    void Update(float deltaTime) override
    {
        // First, we get all the entities with the QuadTransform and Direction components
        auto entities = m_ParentScene->GetEntitiesWith<Nova::QuadTransform, Direction>();

        // Then, we get a reference to the window (we need it to get the window height)
        const auto& window = Nova::App::Get().GetWindow();

        // Finally, we process each entity (the parameters are the components of the entity)
        entities.each([deltaTime, &window](Nova::QuadTransform& transform, Direction& direction) {
            // Check if the entity has reached the top or bottom of the screen
            // If it has, reverse the direction

            const float halfSquareHeight = transform.Scale.y / 2.0f;
            const float windowHeight = window.GetHeight();

            // Positions refer to the center of the square
            // So we need to add/subtract half the size of the square to the position
            // to get the top/bottom of the square

            if (transform.Position.y + halfSquareHeight > windowHeight)
            {
                direction *= -1.0f;
                transform.Position.y = windowHeight - halfSquareHeight;
            }
            else if (transform.Position.y - halfSquareHeight < 0.0f)
            {
                direction *= -1.0f;
                transform.Position.y = halfSquareHeight;
            }

            // Move the entity
            transform.Position.y += SPEED * direction * deltaTime;
        });
    }
};

// This class defines a simple scene for the sandbox project.
// It demonstrates basic usage of the Nova framework: input handling, rendering, and ImGui UI.
// You can experiment with the code to see how it works.
// Currently, the scene just draws a white square that follows the mouse and its size/rotation
// can be controlled with ImGui.
class SandboxScene : public Nova::Scene
{
public:
    // Inherit the constructor from the base class
    // This allows us to call the base class constructor
    // so you don't need to write it yourself
    using Nova::Scene::Scene;

    // This function gets called when you do
    // something like m_SceneManager.StartScene("SandboxScene");
    void Start() override
    {
        Nova::Logger::Info("Hi! Welcome to Nova!");

        // Now we create some entities with the ECS (Entity Component System) that have:
        // - a QuadTransform (position, scale (respect to 1x1 square, so basically their size), rotation)
        // - a ColorComponent (component that stores the color of the quad)
        // - a Direction (stores the direction of movement on the y axis)

        const auto colors = std::to_array<Nova::Color>({
            Nova::Red,
            Nova::Green,
            Nova::Blue,
            Nova::Yellow,
            Nova::Beige,
            Nova::Magenta,
            Nova::Lime,
            Nova::Maroon,
            Nova::White,
            Nova::Black,
        });

        for (int i = 0; i < 10; ++i)
        {
            auto entity = CreateEntity();

            entity.AddComponent<Nova::QuadTransform>(Nova::QuadTransform{
                .Position = {100.0f * (i + 1), 50.0f * (i + 1)},
                .Scale = {50.0f, 50.0f},
                .Rotation = 0.0f,
            });

            entity.AddComponent<Nova::ColorComponent>(colors[i]);
            entity.AddComponent<Direction>((i % 2 == 0) ? 1.0f : -1.0f);
        }

        AddSystem<MovementSystem>();
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

        Nova::Metrics::DebugUI();
    }

private:
    // Here we define the variables that control the white square
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
        // Add and start the SandboxScene
        m_SceneManager.AddScene<SandboxScene>("SandboxScene");
        m_SceneManager.StartScene("SandboxScene");
    }
};

// Nova framework entry point.
// Creates and returns the main application instance with the desired app configuration.
Nova::AppPtr Nova::CreateApp()
{
    Nova::AppConfig config = {
        .Window =
            {
                .Flags = Nova::WindowFlags_Resizable | Nova::WindowFlags_EnableMSAAx4,
                .Width = 1280,
                .Height = 720,
                .Title = "Nova Sandbox project",
            },
    };

    return Nova::MakeApp<SandboxApp>(config);
}
