#pragma once

#include "Buffer.h"
#include "../Material.h"

namespace Engine {

	class InternalMeshAccessor;

	class Mesh
	{
	public:
		friend class InternalMeshAccessor;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
		virtual void SetMaterial(const std::shared_ptr<Material>& material) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual const std::shared_ptr<Material>& GetMaterial() const = 0;

		ENGINE_API static std::shared_ptr<Mesh> Create();

	private:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class InternalMeshAccessor
	{
	public:
		static void Bind(const std::shared_ptr<Mesh>& mesh)
		{
			mesh->Bind();
		}

		static void Unbind(const std::shared_ptr<Mesh>& mesh)
		{
			mesh->Unbind();
		}
	};
}