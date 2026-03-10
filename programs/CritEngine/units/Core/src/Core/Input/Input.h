#pragma once

#include "../Core/Base.h"
#include <unordered_map>
#include "../Event/Event.h"
#include "../Event/KeyboardEvent.h"
#include "../Event/MouseEvent.h"
#include <array>

struct GLFWwindow;

// GLFW keycode wrapper here so we can use it in the sandbox
namespace Keys
{
    enum Key : uint32_t
    {
        Space = 32, 
        Apostrophe = 39, 
        Comma = 44, 
        Minus = 45, 
        Period = 46, 
        Slash = 47, 
        Digit0 = 48, 
        Digit1 = 49, 
        Digit2 = 50, 
        Digit3 = 51, 
        Digit4 = 52, 
        Digit5 = 53, 
        Digit6 = 54, 
        Digit7 = 55, 
        Digit8 = 56, 
        Digit9 = 57, 
        Semicolon = 59, 
        Equal = 61, 
        A = 65, 
        B = 66, 
        C = 67, 
        D = 68, 
        E = 69, 
        F = 70, 
        G = 71, 
        H = 72, 
        I = 73, 
        J = 74, 
        K = 75, 
        L = 76, 
        M = 77, 
        N = 78, 
        O = 79, 
        P = 80, 
        Q = 81, 
        R = 82, 
        S = 83, 
        T = 84, 
        U = 85, 
        V = 86, 
        W = 87, 
        X = 88, 
        Y = 89, 
        Z = 90, 
        LeftBracket = 91, 
        Backslash = 92, 
        RightBracket = 93, 
        GraveAccent = 96, 
        World1 = 161, 
        World2 = 162, 
        Escape = 256, 
        Enter = 257, 
        Tab = 258, 
        Backspace = 259, 
        Insert = 260, 
        DeleteKey = 261, 
        Right = 262, 
        Left = 263, 
        Down = 264, 
        Up = 265, 
        Pageup = 266, 
        Pagedown = 267, 
        Home = 268, 
        End = 269, 
        Capslock = 280, 
        ScrollLock = 281, 
        Numlock = 282, 
        PrintScreen = 283, 
        Pause = 284, 
        F1 = 290, 
        F2 = 291, 
        F3 = 292, 
        F4 = 293, 
        F5 = 294, 
        F6 = 295, 
        F7 = 296, 
        F8 = 297, 
        F9 = 298, 
        F10 = 299, 
        F11 = 300, 
        F12 = 301, 
        F13 = 302, 
        F14 = 303, 
        F15 = 304, 
        F16 = 305, 
        F17 = 306, 
        F18 = 307, 
        F19 = 308, 
        F20 = 309, 
        F21 = 310, 
        F22 = 311, 
        F23 = 312, 
        F24 = 313, 
        F25 = 314, 
        Kp0 = 320, 
        Kp1 = 321, 
        Kp2 = 322, 
        Kp3 = 323, 
        Kp4 = 324, 
        Kp5 = 325, 
        Kp6 = 326, 
        Kp7 = 327, 
        Kp8 = 328, 
        Kp9 = 329, 
        KpDecimal = 330, 
        KpDivide = 331, 
        KpMultiply = 332, 
        KpSubtract = 333, 
        KpAdd = 334, 
        KpEnter = 335, 
        KpEqual = 336, 
        LeftShift = 340, 
        LeftControl = 341, 
        LeftAlt = 342, 
        LeftSuper = 343, 
        RightShift = 344, 
        RightControl = 345, 
        RightAlt = 346, 
        RightSuper = 347, 
        Menu = 348
    };
}

namespace Mouse
{
    enum Button : unsigned int
    {
        Button1, 
        Button2, 
        Button3, 
        Button4, 
        Button5, 
        Button6, 
        Button7, 
        Button8
    };
}

namespace Engine
{
    class InputListener
    {
    public:
        ENGINE_API InputListener(GLFWwindow* window_handle);

        ENGINE_API void pollKeyEvents();
        ENGINE_API void pollMouseEvents();

        ENGINE_API void onKeyPressed(KeyPressedEvent::FunctionType listener);
        ENGINE_API void onKeyJustPressed(KeyJustPressedEvent::FunctionType listener);
        ENGINE_API void onKeyReleased(KeyReleasedEvent::FunctionType listener);

        ENGINE_API void onMouseButtonPressed(MouseButtonPressedEvent::FunctionType listener);
        ENGINE_API void onMouseButtonJustPressed(MouseButtonJustPressedEvent::FunctionType listener);
        ENGINE_API void onMouseButtonReleased(MouseButtonReleasedEvent::FunctionType listener);
        ENGINE_API void onMouseMove(MouseMoveEvent::FunctionType listener);

    private:
        ENGINE_API static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        ENGINE_API static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        ENGINE_API static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

        EventEmitter m_eventEmitter;

        static std::unordered_map<int, bool> m_buttonStates;
        static std::unordered_map<int, bool> m_buttonReleased;
        static std::unordered_map<int, bool> m_buttonJustPressed;
        static std::unordered_map<int, bool> m_keyStates;
        static std::unordered_map<int, bool> m_keyReleased;
        static std::unordered_map<int, bool> m_keyJustPressed;

        static Vector2 m_curPos;
        static Vector2 m_prevCurPos;

        static bool m_curPosChanged;

        static double m_posX;
        static double m_posY;
    };

    constexpr unsigned int mouseButton(Mouse::Button button)
    {
        return static_cast<int>(button);
    }

    constexpr int keyCode(Keys::Key key)
    {
        return  static_cast<int>(key);
    }
}
