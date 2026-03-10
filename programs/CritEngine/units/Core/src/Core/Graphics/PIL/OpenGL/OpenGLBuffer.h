#pragma once
#include "../Buffer.h"

namespace Engine
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        void bind() const override;
        void unbind() const override;

        const BufferLayout& getLayout() const override { return this->m_layout; }
        void setLayout(BufferLayout layout) override { this->m_layout = layout; }

    private:
        uint32_t m_bufferID;
        BufferLayout m_layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* vertices, uint32_t count);
        ~OpenGLIndexBuffer() override;

        void bind() const override;
        void unbind() const override;

        uint32_t getCount() const override { return this->m_count; }

    private:
        uint32_t m_bufferID;
        uint32_t m_count;
    };
}
