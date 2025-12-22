#include "Window.h"
#include "../Core/GlobalEngine.h"

#include <GLFW/glfw3.h>

namespace Engine {

	Window::Window(const int width, const int height, const std::string& title)
		: eventEmitter(EventEmitter()), width(width), height(height)
	{
		bool success = glfwInit();
		ASSERT(success, "Failed to initialize GLFW!");

		// We can use whatever version we want, I just set it to the latest
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		this->windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		ASSERT(this->windowHandle, "Failed to create GLFW window!");

		glfwSetInputMode(this->windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetWindowUserPointer(this->windowHandle, this);

		this->input = std::make_unique<InputListener>(this->GetHandle());
		this->renderContext = RenderContext::Create(this->GetHandle());
		this->renderContext->Init();
		this->renderContext->InitImGui();

		glfwSetWindowCloseCallback(this->windowHandle, [](GLFWwindow* glfwWindow)
		{
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
			window->eventEmitter.Emit<WindowCloseEvent>(window);
		});

		this->eventEmitter.AddListener<WindowCloseEvent>([](Window* window)
		{
			GlobalEngine::Shutdown();
		});

		glfwSetWindowSizeCallback(this->windowHandle, [](GLFWwindow* glfwWindow, int width, int height) 
		{
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));		
			window->eventEmitter.Emit<WindowResizeEvent>(window, width, height);
		});

		this->eventEmitter.AddListener<WindowResizeEvent>([](Window* window, int width, int height)
		{
			window->SetWidth(width);
			window->SetHeight(height);

			RenderCommand::SetViewportSize(width, height);

		});
		
	}

	Window::~Window()
	{
		glfwDestroyWindow(this->windowHandle);
		glfwTerminate();
	}

	GLFWwindow* Window::GetHandle()
	{
		return this->windowHandle;
	}

	void Window::Tick()
	{
		this->GetInput()->PollKeyEvents();
		this->GetInput()->PollMouseEvents();
		this->PollEvents();
		this->SwapBuffers();
	}

	void Window::SwapBuffers()
	{
		this->renderContext->SwapBuffers();
	}

	void Window::ImGuiStartFrame()
	{
		this->renderContext->ImGuiStartFrame();
	}

	void Window::ImGuiRender()
	{
		this->renderContext->ImGuiRender();
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	std::shared_ptr<InputListener> Window::GetInput()
	{
		ASSERT(this->input);
		return this->input;
	}
};