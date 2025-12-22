#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Engine {

	/////////////////////////////////////////////////////////////////////
    //// VertexBuffer ///////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		// We create and assign the buffer data locally instead of having to bind and assign data within the global state.
		glCreateBuffers(1, &this->bufferID);
		glNamedBufferData(this->bufferID, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &this->bufferID);
	};

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	/////////////////////////////////////////////////////////////////////
	//// IndexBuffer ////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////


	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: count(count)
	{
		glCreateBuffers(1, &this->bufferID);
		glNamedBufferData(this->bufferID, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &this->bufferID);
	};

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}
}