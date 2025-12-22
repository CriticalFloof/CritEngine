#include "Input.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Window/Window.h"

namespace Engine {

    std::unordered_map<int, bool> InputListener::KeyStates; 
    std::unordered_map<int, bool> InputListener::KeyReleased;
    std::unordered_map<int, bool> InputListener::KeyJustPressed;
    std::unordered_map<int, bool> InputListener::ButtonStates;
    std::unordered_map<int, bool> InputListener::ButtonReleased; 
    std::unordered_map<int, bool> InputListener::ButtonJustPressed;
    Vector2 InputListener::CurPos;
    Vector2 InputListener::PrevCurPos;
    bool InputListener::CurPosChanged = false;

    InputListener::InputListener(GLFWwindow* windowHandle) : eventEmitter(EventEmitter()) {
        glfwSetKeyCallback(windowHandle, this->KeyCallback);
        glfwSetMouseButtonCallback(windowHandle, this->MouseButtonCallback);
        glfwSetCursorPosCallback(windowHandle, this->CursorPosCallback);
    }

    void InputListener::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

        if (action == GLFW_PRESS) {
            KeyStates[key] = true;
            KeyReleased[key] = false;
            KeyJustPressed[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            KeyStates[key] = false;
            KeyReleased[key] = true;
        }
    }

    void InputListener::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {

        if (action == GLFW_PRESS) {
            ButtonStates[button] = true;
            ButtonReleased[button] = false;
            ButtonJustPressed[button] = true;
        }
        else if (action == GLFW_RELEASE) {
            ButtonStates[button] = false;
            ButtonReleased[button] = true;
        }

    }

    void InputListener::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (CurPos.x != xpos || CurPos.y != ypos) {
            PrevCurPos = CurPos;
            CurPos.x = static_cast<float>(xpos);
            CurPos.y = static_cast<float>(ypos);
            CurPosChanged = true;
        }
        else {
            CurPosChanged = false;
        }
    }

    void InputListener::PollKeyEvents() {

        for (const auto& [key, pressed] : KeyStates) {
            if (pressed) {
                this->eventEmitter.Emit<KeyPressedEvent>((unsigned int)key);
            }
        }

        for (const auto& [key, released] : KeyReleased) {
            if (released) {
                this->eventEmitter.Emit<KeyReleasedEvent>((unsigned int)key);
                KeyReleased[key] = false;
            }
        }

        for (const auto& [key, justPressed] : KeyJustPressed) {
            if (justPressed) {
                this->eventEmitter.Emit<KeyJustPressedEvent>((unsigned int)key);
                KeyJustPressed[key] = false;
            }
        }

    }

    void InputListener::PollMouseEvents()
    {

        for (const auto& [button, pressed] : ButtonStates)
        {
            if (pressed)
            {
                this->eventEmitter.Emit<MouseButtonPressedEvent>((unsigned int)button);
            }
        }

        for (const auto& [button, released] : ButtonReleased)
        {
            if (released)
            {
                this->eventEmitter.Emit<MouseButtonReleasedEvent>((unsigned int)button);
                ButtonReleased[button] = false;
            }
        }

        for (const auto& [button, justPressed] : ButtonJustPressed)
        {
            if (justPressed)
            {
                this->eventEmitter.Emit<MouseButtonJustPressedEvent>((unsigned int)button);
                ButtonJustPressed[button] = false;
            }
        }

        if (this->CurPosChanged)
        {
            this->eventEmitter.Emit<MouseMoveEvent>(this->CurPos);
            CurPosChanged = false;
        }
    }

    void InputListener::OnKeyPressed(KeyPressedEvent::FunctionType listener)
    {
        this->eventEmitter.AddListener<KeyPressedEvent>(listener);
    }
    void InputListener::OnKeyJustPressed(KeyJustPressedEvent::FunctionType listener)
    {
        this->eventEmitter.AddListener<KeyJustPressedEvent>(listener);
    }
    void InputListener::OnKeyReleased(KeyReleasedEvent::FunctionType listener)
    {
        this->eventEmitter.AddListener<KeyReleasedEvent>(listener);
    }
    void InputListener::OnMouseButtonPressed(MouseButtonPressedEvent::FunctionType listener)
    {
        this->eventEmitter.AddListener<MouseButtonPressedEvent>(listener);
    }
    void InputListener::OnMouseButtonJustPressed(MouseButtonJustPressedEvent::FunctionType listener)
    {
        this->eventEmitter.AddListener<MouseButtonJustPressedEvent>(listener);
    }
    void InputListener::OnMouseButtonReleased(MouseButtonReleasedEvent::FunctionType listener)
    {
        this->eventEmitter.AddListener<MouseButtonReleasedEvent>(listener);
    }
    void InputListener::OnMouseMove(MouseMoveEvent::FunctionType listener)
    {
        this->eventEmitter.AddListener<MouseMoveEvent>(listener);
    }

}
