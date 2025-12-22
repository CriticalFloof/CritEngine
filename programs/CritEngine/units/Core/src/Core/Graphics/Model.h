#pragma once
#include "PIL/Mesh.h"

namespace Engine {

	class Model
	{
	public:
		Model(std::shared_ptr<Mesh> mesh, Matrix4f projection = Matrix4f::Identity())
			: mesh(mesh), modelProjection(projection) {};

		void SetMesh(std::shared_ptr<Mesh> newMesh) { this->mesh = newMesh; }
		void SetProjection(Matrix4f projection) { this->modelProjection = projection; }
		
		std::shared_ptr<Mesh> GetMesh() { return this->mesh; }
		Matrix4f& GetModelProjection() { return this->modelProjection; }

	private:
		std::shared_ptr<Mesh> mesh;
		Matrix4f modelProjection;
	};

}