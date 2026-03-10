#include "Renderer.h"

namespace Engine
{
    std::shared_ptr<BaseCamera> Renderer::m_activeCamera = std::make_shared<PerspectiveCamera>();

    void Renderer::beginScene(std::shared_ptr<BaseCamera> camera)
    {
        m_activeCamera = camera;
    }

    void Renderer::endScene()
    {
    }

    void Renderer::submit(const std::shared_ptr<Model>& model)
    {
        std::shared_ptr<Mesh> mesh = model->getMesh();
        std::shared_ptr<Pipeline> program = InternalMaterialAccessor::getPipeline(mesh->getMaterial());
        program->bind();
        program->uploadUniformMat4("uViewProjection", m_activeCamera->getViewMatrix());
        program->uploadUniformMat4("uPerspectiveProjection", m_activeCamera->getPerspectiveMatrix());
        program->uploadUniformMat4("uViewPerspectiveProjection", m_activeCamera->getViewPerspectiveMatrix());
        program->uploadUniformMat4("uModelProjection", model->getModelProjection());

        std::vector<std::shared_ptr<Texture>> textures = InternalMaterialAccessor::getTextures(mesh->getMaterial());
        for (size_t i = 0; i < textures.size(); i++)
        {
            textures[i]->bind(i);
        }

        InternalMeshAccessor::bind(mesh);
        RenderCommand::drawIndexed(mesh);
    }
}
