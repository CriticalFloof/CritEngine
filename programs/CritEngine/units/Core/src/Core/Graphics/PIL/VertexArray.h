#pragma once

#include "../../Core/Base.h"
#include "Buffer.h"

namespace Engine
{
    class VertexArray
    {
    public:
        virtual ~VertexArray()
        {
        };

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) = 0;
        virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;

        ENGINE_API static std::shared_ptr<VertexArray> create();
    };
}
