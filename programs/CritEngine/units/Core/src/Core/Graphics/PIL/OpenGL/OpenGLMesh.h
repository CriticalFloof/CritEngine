#pragma once
#include "../Mesh.h"

namespace Engine {

	class OpenGLMesh : public Mesh
	{
	public:
		OpenGLMesh();
		virtual ~OpenGLMesh() {};

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		virtual void SetMaterial(const std::shared_ptr<Material>& material) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return this->vertexBuffers; } 
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return this->indexBuffer; }
		virtual const std::shared_ptr<Material>& GetMaterial() const override { return this->material; }

	private:
		virtual void Bind() const override;
		virtual void Unbind() const override;

		uint32_t vertexArrayID;
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<IndexBuffer> indexBuffer;
		std::shared_ptr<Material> material;
	};

}