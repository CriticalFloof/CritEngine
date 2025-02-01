#include "Graphics.h"
#include "Core/MainLoop.h" // Need this to reference the window context we created
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {


	GraphicsRenderer::GraphicsRenderer(Engine::Window& window_ref) 
		: windowHandle(window_ref.GetHandle())
	{
		glfwMakeContextCurrent(this->windowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LogError("GLAD", "Failed to initialize GLAD!");
			exit(EXIT_FAILURE);
		}

		glfwSwapInterval(1); // Wait on 1 monitor refresh before swapping buffers / VSync
	}

	GraphicsRenderer::~GraphicsRenderer()
	{

	}

	// To be replaced with more relevant stuff here, but keeping this below for testing
	//
	void GraphicsRenderer::Draw()
	{
		glClearColor(0.6f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(windowHandle);
	}

}