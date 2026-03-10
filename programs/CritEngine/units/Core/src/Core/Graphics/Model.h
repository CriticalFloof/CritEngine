#pragma once
#include "PIL/Mesh.h"

namespace Engine
{
    class Model
    {
    public:
        Model(std::shared_ptr<Mesh> mesh, Matrix4f projection = Matrix4f::identity())
            : m_mesh(mesh), m_modelProjection(projection)
        {
        };

        void setMesh(std::shared_ptr<Mesh> newMesh) { this->m_mesh = newMesh; }
        void setProjection(Matrix4f projection) { this->m_modelProjection = projection; }

        std::shared_ptr<Mesh> getMesh() { return this->m_mesh; }
        Matrix4f& getModelProjection() { return this->m_modelProjection; }

    private:
        std::shared_ptr<Mesh> m_mesh;
        Matrix4f m_modelProjection;
    };
}
