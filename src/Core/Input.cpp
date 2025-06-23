#include "Nova/Core/Input.hpp"
#include "Nova/Misc/Logger.hpp"
#include "Nova/Core/Window.hpp"

#include <GLFW/glfw3.h>
#include <array>
#include <glm/vec2.hpp>
#include <glm/ext/vector_float2.hpp>

namespace Nova::Input
{
    static std::array<bool, (size_t) Key::Last + 1> s_CurrentKeyState;
    static std::array<bool, (size_t) Key::Last + 1> s_PreviousKeyState;

    static std::array<bool, (size_t) MouseButton::Last + 1> s_CurrentMouseState;
    static std::array<bool, (size_t) MouseButton::Last + 1> s_PreviousMouseState;

    static glm::vec2 s_MouseWheelOffset;

    void PollEvents()
    {
        s_PreviousKeyState = s_CurrentKeyState;
        s_PreviousMouseState = s_CurrentMouseState;
        s_MouseWheelOffset = glm::vec2(0.0f);

        glfwPollEvents();
    }

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key < 0)
            return;

        size_t k = (size_t) key;

        s_CurrentKeyState[k] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button < 0)
            return;

        size_t k = (size_t) button;

        s_CurrentMouseState[k] = (action == GLFW_PRESS);
    }

    void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
    {
        s_MouseWheelOffset = glm::vec2((float) xOffset, (float) yOffset);
    }

    bool IsKeyUp(Key key)
    {
        size_t k = (size_t) key;

        if (k > (size_t) Key::Last)
        {
            Logger::Warning("Tried to access an invalid key value");
            return false;
        }

        return !s_CurrentKeyState[k];
    }

    bool IsKeyDown(Key key)
    {
        size_t k = (size_t) key;

        if (k > (size_t) Key::Last)
        {
            Logger::Warning("Tried to access an invalid key value");
            return false;
        }

        return s_CurrentKeyState[k];
    }

    bool IsKeyPressed(Key key)
    {
        size_t k = (size_t) key;

        if (k > (size_t) Key::Last)
        {
            Logger::Warning("Tried to access an invalid key value");
            return false;
        }

        return !s_PreviousKeyState[k] && s_CurrentKeyState[k];
    }

    bool IsMouseButtonUp(MouseButton button)
    {
        size_t k = (size_t) button;

        if (k > (size_t) MouseButton::Last)
        {
            Logger::Warning("Tried to access an invalid button value");
            return false;
        }

        return !s_CurrentMouseState[k];
    }

    bool IsMouseButtonDown(MouseButton button)
    {
        size_t k = (size_t) button;

        if (k > (size_t) MouseButton::Last)
        {
            Logger::Warning("Tried to access an invalid button value");
            return false;
        }

        return s_CurrentMouseState[k];
    }

    bool IsMouseButtonPressed(MouseButton button)
    {
        size_t k = (size_t) button;

        if (k > (size_t) MouseButton::Last)
        {
            Logger::Warning("Tried to access an invalid button value");
            return false;
        }

        return !s_PreviousMouseState[k] && s_CurrentMouseState[k];
    }

    glm::vec2 GetMousePos()
    {
        double x, y;
        glfwGetCursorPos(Window::Get().GetNativeWindow(), &x, &y);
        return {(float) x, (float) y};
    }

    glm::vec2 GetMouseWheel()
    {
        return s_MouseWheelOffset;
    }
} // namespace Nova::Input
