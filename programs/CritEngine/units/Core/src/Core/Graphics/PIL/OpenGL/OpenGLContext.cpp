#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
    {
        this->m_windowHandle = window_handle;
    }

    OpenGLContext::~OpenGLContext()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }

    void OpenGLContext::init()
    {
        glfwMakeContextCurrent(this->m_windowHandle);
        bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ASSERT(success, "Failed to initialize GLAD!");
    }

    void OpenGLContext::swapBuffers()
    {
        glfwSwapBuffers(this->m_windowHandle);
    }

    void OpenGLContext::initImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplGlfw_InitForOpenGL(this->m_windowHandle, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void OpenGLContext::imGuiStartFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void OpenGLContext::imGuiRender()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
