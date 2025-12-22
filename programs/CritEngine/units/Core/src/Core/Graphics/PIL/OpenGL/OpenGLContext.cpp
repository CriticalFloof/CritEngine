#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	{
		this->windowHandle = windowHandle;
	}

	OpenGLContext::~OpenGLContext()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(this->windowHandle);
		bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(success, "Failed to initialize GLAD!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(this->windowHandle);
	}

	void OpenGLContext::InitImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui_ImplGlfw_InitForOpenGL(this->windowHandle, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void OpenGLContext::ImGuiStartFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void OpenGLContext::ImGuiRender()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}