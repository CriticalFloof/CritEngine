#include "OpenGLMesh.h"
#include <glad/glad.h>

namespace Engine
{
    static GLenum shaderDataTypeToOpenGLDataType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::None: ASSERT(false, "'None' is an Invalid ShaderDataType.");
            return 0;
        case ShaderDataType::Float: return GL_FLOAT;
        case ShaderDataType::Float2: return GL_FLOAT;
        case ShaderDataType::Float3: return GL_FLOAT;
        case ShaderDataType::Float4: return GL_FLOAT;
        case ShaderDataType::Mat3: return GL_FLOAT;
        case ShaderDataType::Mat4: return GL_FLOAT;
        case ShaderDataType::Int: return GL_INT;
        case ShaderDataType::Int2: return GL_INT;
        case ShaderDataType::Int3: return GL_INT;
        case ShaderDataType::Int4: return GL_INT;
        case ShaderDataType::Bool: return GL_BOOL;
        }

        ASSERT(false, "Unknown ShaderDataType.");
        return 0;
    }

    OpenGLMesh::OpenGLMesh()
    {
        glCreateVertexArrays(1, &this->m_vertexArrayID);
    }

    void OpenGLMesh::bind() const
    {
        glBindVertexArray(this->m_vertexArrayID);
    }

    void OpenGLMesh::unbind() const
    {
        glBindVertexArray(NULL);
    }

    void OpenGLMesh::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer)
    {
        ASSERT(vertex_buffer->getLayout().getElements().size(), "Vertex Buffer has no layout!")

        glBindVertexArray(this->m_vertexArrayID);
        vertex_buffer->bind();

        uint32_t layout_index = 0;
        const auto& layout = vertex_buffer->getLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(layout_index);
            glVertexAttribPointer
            (layout_index,
             element.getComponentCount(),
             shaderDataTypeToOpenGLDataType(element.type),
             element.normalized ? GL_TRUE : GL_FALSE,
             layout.getStride(),
             (const void*)element.offset);
            layout_index++;
        }

        this->m_vertexBuffers.push_back(vertex_buffer);
    }

    void OpenGLMesh::setIndexBuffer(const std::shared_ptr<IndexBuffer>& new_index_buffer)
    {
        glBindVertexArray(this->m_vertexArrayID);
        new_index_buffer->bind();
        this->m_indexBuffer = new_index_buffer;
    }

    void OpenGLMesh::setMaterial(const std::shared_ptr<Material>& new_material)
    {
        this->m_material = new_material;
    }
}
