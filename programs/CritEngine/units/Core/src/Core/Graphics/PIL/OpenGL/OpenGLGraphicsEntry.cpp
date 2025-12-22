#include "OpenGLGraphicsEntry.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

	void OpenGLGraphicsEntry::Init()
	{
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		GLFWwindow* dummy = glfwCreateWindow(1, 1, "", nullptr, nullptr);
		glfwMakeContextCurrent(dummy);
		bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(success, "Failed to initialize GLAD!");

		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	}

}