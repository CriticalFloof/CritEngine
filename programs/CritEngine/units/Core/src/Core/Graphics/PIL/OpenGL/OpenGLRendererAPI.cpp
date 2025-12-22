#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Engine {

	static GLenum DepthFuncTypeToOpenGLDepthFuncType(Engine::RendererAPI::DepthTestFunction type)
	{
		switch (type)
		{
			case Engine::RendererAPI::DepthTestFunction::Always:  return GL_ALWAYS;
			case Engine::RendererAPI::DepthTestFunction::Never:  return GL_NEVER;
			case Engine::RendererAPI::DepthTestFunction::Greater:  return GL_GREATER;
			case Engine::RendererAPI::DepthTestFunction::GreaterOrEqual:  return GL_GEQUAL;
			case Engine::RendererAPI::DepthTestFunction::Less:  return GL_LESS;
			case Engine::RendererAPI::DepthTestFunction::LessOrEqual:  return GL_LEQUAL;
			case Engine::RendererAPI::DepthTestFunction::Equal:  return GL_EQUAL;
			case Engine::RendererAPI::DepthTestFunction::NotEqual:  return GL_NOTEQUAL;
		}

		ASSERT(false, "Unknown ShaderDataType.")
			return 0;
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::EnableDepthTest(const bool newState)
	{
		newState ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::EnableDepthMask(const bool newState)
	{
		glDepthMask(newState);
	}

	void OpenGLRendererAPI::SetDepthTestFunc(const DepthTestFunction newFunction)
	{
		glDepthFunc(DepthFuncTypeToOpenGLDepthFuncType(newFunction));
	}

	void OpenGLRendererAPI::SetViewportSize(const int width, const int height)
	{
		glViewport(0, 0, width, height);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<Mesh>& mesh)
	{
		glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}