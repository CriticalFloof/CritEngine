#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Engine
{
    /////////////////////////////////////////////////////////////////////
    //// VertexBuffer ///////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        // We create and assign the buffer data locally instead of having to bind and assign data within the global state.
        glCreateBuffers(1, &this->m_bufferID);
        glNamedBufferData(this->m_bufferID, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &this->m_bufferID);
    };

    void OpenGLVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->m_bufferID);
    }

    void OpenGLVertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
    }

    /////////////////////////////////////////////////////////////////////
    //// IndexBuffer ////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////


    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        : m_count(count)
    {
        glCreateBuffers(1, &this->m_bufferID);
        glNamedBufferData(this->m_bufferID, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &this->m_bufferID);
    };

    void OpenGLIndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_bufferID);
    }

    void OpenGLIndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    }
}
