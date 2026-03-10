#pragma once

#include "../Event/Event.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/PIL/RenderContext.h"
#include "../Input/Input.h"

struct GLFWwindow;

namespace Engine
{
    class Window
    {
    public:
        ENGINE_API Window(int width, int height, const std::string& title);
        ENGINE_API ~Window();

        ENGINE_API GLFWwindow* getHandle();
        ENGINE_API std::shared_ptr<InputListener> getInput();

        ENGINE_API void setWidth(const int width) { m_width = width; }
        ENGINE_API void setHeight(const int height) { m_height = height; }

        ENGINE_API int getWidth() { return m_width; }
        ENGINE_API int getHeight() { return m_height; }
        ENGINE_API float getAspectRatio() { return static_cast<float>(m_width) / static_cast<float>(m_height); }

        ENGINE_API void imGuiStartFrame();
        ENGINE_API void imGuiRender();

        void tick();

    private:
        void pollEvents();
        void swapBuffers();

        EventEmitter m_eventEmitter;
        GLFWwindow* m_windowHandle = nullptr;
        std::shared_ptr<InputListener> m_input = nullptr;
        std::shared_ptr<RenderContext> m_renderContext = nullptr;
        int m_width;
        int m_height;
    };

    struct WindowEvent : Event<std::tuple<>, AnyEvent>
    {
    };

    struct WindowResizeEvent : Event<std::tuple<Window*, int, int>, WindowEvent>
    {
    };

    struct WindowCloseEvent : Event<std::tuple<Window*>, WindowEvent>
    {
    };
};
