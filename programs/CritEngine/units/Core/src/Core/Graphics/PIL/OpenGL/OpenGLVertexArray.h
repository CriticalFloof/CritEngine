#pragma once
#include "../VertexArray.h"

namespace Engine
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();

        ~OpenGLVertexArray() override
        {
        };

        void bind() const override;
        void unbind() const override;

        void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) override;
        void setIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) override;

        const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override
        {
            return this->m_vertexBuffers;
        }

        const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override { return this->m_indexBuffer; }

    private:
        uint32_t m_vertexArrayID;
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
    };
}
