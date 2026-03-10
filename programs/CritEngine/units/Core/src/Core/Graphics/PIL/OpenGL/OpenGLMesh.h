#pragma once
#include "../Mesh.h"

namespace Engine
{
    class OpenGLMesh : public Mesh
    {
    public:
        OpenGLMesh();

        virtual ~OpenGLMesh()
        {
        };

        void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) override;
        void setIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) override;
        void setMaterial(const std::shared_ptr<Material>& material) override;

        const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override
        {
            return this->m_vertexBuffers;
        }

        const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override { return this->m_indexBuffer; }
        const std::shared_ptr<Material>& getMaterial() const override { return this->m_material; }

    private:
        void bind() const override;
        void unbind() const override;

        uint32_t m_vertexArrayID;
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        std::shared_ptr<Material> m_material;
    };
}
