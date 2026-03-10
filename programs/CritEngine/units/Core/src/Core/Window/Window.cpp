#include "Window.h"
#include "../Core/GlobalEngine.h"

#include <GLFW/glfw3.h>

namespace Engine
{
    Window::Window(const int width, const int height, const std::string& title)
        : m_eventEmitter(EventEmitter()), m_width(width), m_height(height)
    {
        bool success = glfwInit();
        ASSERT(success, "Failed to initialize GLFW!");

        // We can use whatever version we want, I just set it to the latest
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        ASSERT(m_windowHandle, "Failed to create GLFW window!");

        glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetWindowUserPointer(m_windowHandle, this);

        m_input = std::make_unique<InputListener>(this->getHandle());
        m_renderContext = RenderContext::create(this->getHandle());
        m_renderContext->init();
        m_renderContext->initImGui();

        glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow* glfw_window)
        {
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
            window->m_eventEmitter.emit<WindowCloseEvent>(window);
        });

        m_eventEmitter.addListener<WindowCloseEvent>([](Window* window)
        {
            GlobalEngine::shutdown();
        });

        glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow* glfw_window, int width, int height)
        {
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
            window->m_eventEmitter.emit<WindowResizeEvent>(window, width, height);
        });

        m_eventEmitter.addListener<WindowResizeEvent>([](Window* window, int width, int height)
        {
            window->setWidth(width);
            window->setHeight(height);

            RenderCommand::setViewportSize(width, height);
        });
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_windowHandle);
        glfwTerminate();
    }

    GLFWwindow* Window::getHandle()
    {
        return m_windowHandle;
    }

    void Window::tick()
    {
        this->getInput()->pollKeyEvents();
        this->getInput()->pollMouseEvents();
        this->pollEvents();
        this->swapBuffers();
    }

    void Window::swapBuffers()
    {
        m_renderContext->swapBuffers();
    }

    void Window::imGuiStartFrame()
    {
        m_renderContext->imGuiStartFrame();
    }

    void Window::imGuiRender()
    {
        m_renderContext->imGuiRender();
    }

    void Window::pollEvents()
    {
        glfwPollEvents();
    }

    std::shared_ptr<InputListener> Window::getInput()
    {
        ASSERT(this->m_input)
        
        return this->m_input;
    }
};
