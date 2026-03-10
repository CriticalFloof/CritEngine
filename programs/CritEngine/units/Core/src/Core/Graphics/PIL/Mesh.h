#pragma once

#include "Buffer.h"
#include "../Material.h"

namespace Engine
{
    class InternalMeshAccessor;

    class Mesh
    {
    public:
        friend class InternalMeshAccessor;

        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) = 0;
        virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) = 0;
        virtual void setMaterial(const std::shared_ptr<Material>& material) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;
        virtual const std::shared_ptr<Material>& getMaterial() const = 0;

        ENGINE_API static std::shared_ptr<Mesh> create();

    private:
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
    };

    class InternalMeshAccessor
    {
    public:
        static void bind(const std::shared_ptr<Mesh>& mesh)
        {
            mesh->bind();
        }

        static void unbind(const std::shared_ptr<Mesh>& mesh)
        {
            mesh->unbind();
        }
    };
}
