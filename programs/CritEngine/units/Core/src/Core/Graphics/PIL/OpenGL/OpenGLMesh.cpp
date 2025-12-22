#include "OpenGLMesh.h"
#include <glad/glad.h>

namespace Engine {

	static GLenum ShaderDataTypeToOpenGLDataType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None: ASSERT(false, "'None' is an Invalid ShaderDataType."); return 0;
			case ShaderDataType::Float:  return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Mat3:   return GL_FLOAT;
			case ShaderDataType::Mat4:   return GL_FLOAT;
			case ShaderDataType::Int:    return GL_INT;
			case ShaderDataType::Int2:   return GL_INT;
			case ShaderDataType::Int3:   return GL_INT;
			case ShaderDataType::Int4:   return GL_INT;
			case ShaderDataType::Bool:   return GL_BOOL;
		}

		ASSERT(false, "Unknown ShaderDataType.");
		return 0;
	}

	OpenGLMesh::OpenGLMesh()
	{
		glCreateVertexArrays(1, &this->vertexArrayID);
	}

	void OpenGLMesh::Bind() const
	{
		glBindVertexArray(this->vertexArrayID);
	}

	void OpenGLMesh::Unbind() const
	{
		glBindVertexArray(NULL);
	}

	void OpenGLMesh::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!")

		glBindVertexArray(this->vertexArrayID);
		vertexBuffer->Bind();

		uint32_t layoutIndex = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(layoutIndex);
			glVertexAttribPointer
			(layoutIndex,
			 element.GetComponentCount(),
			 ShaderDataTypeToOpenGLDataType(element.type),
			 element.normalized ? GL_TRUE : GL_FALSE,
			 layout.GetStride(),
			 (const void*)element.offset);
			layoutIndex++;
		}

		this->vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLMesh::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& newIndexBuffer)
	{
		glBindVertexArray(this->vertexArrayID);
		newIndexBuffer->Bind();
		this->indexBuffer = newIndexBuffer;
	}

	void OpenGLMesh::SetMaterial(const std::shared_ptr<Material>& newMaterial)
	{
		this->material = newMaterial;
	}

}