#include "Input.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Window/Window.h"

namespace Engine
{
    std::unordered_map<int, bool> InputListener::m_keyStates;
    std::unordered_map<int, bool> InputListener::m_keyReleased;
    std::unordered_map<int, bool> InputListener::m_keyJustPressed;
    std::unordered_map<int, bool> InputListener::m_buttonStates;
    std::unordered_map<int, bool> InputListener::m_buttonReleased;
    std::unordered_map<int, bool> InputListener::m_buttonJustPressed;
    Vector2 InputListener::m_curPos;
    Vector2 InputListener::m_prevCurPos;
    bool InputListener::m_curPosChanged = false;

    InputListener::InputListener(GLFWwindow* window_handle) : m_eventEmitter(EventEmitter())
    {
        glfwSetKeyCallback(window_handle, keyCallback);
        glfwSetMouseButtonCallback(window_handle, mouseButtonCallback);
        glfwSetCursorPosCallback(window_handle, cursorPosCallback);
    }

    void InputListener::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            m_keyStates[key] = true;
            m_keyReleased[key] = false;
            m_keyJustPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_keyStates[key] = false;
            m_keyReleased[key] = true;
        }
    }

    void InputListener::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            m_buttonStates[button] = true;
            m_buttonReleased[button] = false;
            m_buttonJustPressed[button] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_buttonStates[button] = false;
            m_buttonReleased[button] = true;
        }
    }

    void InputListener::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (m_curPos.x != xpos || m_curPos.y != ypos)
        {
            m_prevCurPos = m_curPos;
            m_curPos.x = static_cast<float>(xpos);
            m_curPos.y = static_cast<float>(ypos);
            m_curPosChanged = true;
        }
        else
        {
            m_curPosChanged = false;
        }
    }

    void InputListener::pollKeyEvents()
    {
        for (const auto& [key, pressed] : m_keyStates)
        {
            if (pressed)
            {
                this->m_eventEmitter.emit<KeyPressedEvent>(static_cast<unsigned int>(key));
            }
        }

        for (const auto& [key, released] : m_keyReleased)
        {
            if (released)
            {
                this->m_eventEmitter.emit<KeyReleasedEvent>(static_cast<unsigned int>(key));
                m_keyReleased[key] = false;
            }
        }

        for (const auto& [key, justPressed] : m_keyJustPressed)
        {
            if (justPressed)
            {
                this->m_eventEmitter.emit<KeyJustPressedEvent>(static_cast<unsigned int>(key));
                m_keyJustPressed[key] = false;
            }
        }
    }

    void InputListener::pollMouseEvents()
    {
        for (const auto& [button, pressed] : m_buttonStates)
        {
            if (pressed)
            {
                this->m_eventEmitter.emit<MouseButtonPressedEvent>(static_cast<unsigned int>(button));
            }
        }

        for (const auto& [button, released] : m_buttonReleased)
        {
            if (released)
            {
                this->m_eventEmitter.emit<MouseButtonReleasedEvent>(static_cast<unsigned int>(button));
                m_buttonReleased[button] = false;
            }
        }

        for (const auto& [button, justPressed] : m_buttonJustPressed)
        {
            if (justPressed)
            {
                this->m_eventEmitter.emit<MouseButtonJustPressedEvent>(static_cast<unsigned int>(button));
                m_buttonJustPressed[button] = false;
            }
        }

        if (m_curPosChanged)
        {
            this->m_eventEmitter.emit<MouseMoveEvent>(m_curPos);
            m_curPosChanged = false;
        }
    }

    void InputListener::onKeyPressed(KeyPressedEvent::FunctionType listener)
    {
        this->m_eventEmitter.addListener<KeyPressedEvent>(listener);
    }

    void InputListener::onKeyJustPressed(KeyJustPressedEvent::FunctionType listener)
    {
        this->m_eventEmitter.addListener<KeyJustPressedEvent>(listener);
    }

    void InputListener::onKeyReleased(KeyReleasedEvent::FunctionType listener)
    {
        this->m_eventEmitter.addListener<KeyReleasedEvent>(listener);
    }

    void InputListener::onMouseButtonPressed(MouseButtonPressedEvent::FunctionType listener)
    {
        this->m_eventEmitter.addListener<MouseButtonPressedEvent>(listener);
    }

    void InputListener::onMouseButtonJustPressed(MouseButtonJustPressedEvent::FunctionType listener)
    {
        this->m_eventEmitter.addListener<MouseButtonJustPressedEvent>(listener);
    }

    void InputListener::onMouseButtonReleased(MouseButtonReleasedEvent::FunctionType listener)
    {
        this->m_eventEmitter.addListener<MouseButtonReleasedEvent>(listener);
    }

    void InputListener::onMouseMove(MouseMoveEvent::FunctionType listener)
    {
        this->m_eventEmitter.addListener<MouseMoveEvent>(listener);
    }
}
