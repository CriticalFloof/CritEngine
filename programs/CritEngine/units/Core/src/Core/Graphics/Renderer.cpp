#include "Renderer.h" 

namespace Engine {

	std::shared_ptr<BaseCamera> Renderer::activeCamera = std::make_shared<PerspectiveCamera>();

	void Renderer::BeginScene(std::shared_ptr<BaseCamera> camera)
	{
		Renderer::activeCamera = camera;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Model>& model)
	{
		std::shared_ptr<Mesh> mesh = model->GetMesh();
		std::shared_ptr<Pipeline> program = InternalMaterialAccessor::GetPipeline(mesh->GetMaterial());
		program->Bind();
		program->UploadUniformMat4("uViewProjection", Renderer::activeCamera->getViewMatrix());
		program->UploadUniformMat4("uPerspectiveProjection", Renderer::activeCamera->getPerspectiveMatrix());
		program->UploadUniformMat4("uViewPerspectiveProjection", Renderer::activeCamera->getViewPerspectiveMatrix());
		program->UploadUniformMat4("uModelProjection", model->GetModelProjection());

		std::vector<std::shared_ptr<Texture>> textures = InternalMaterialAccessor::GetTextures(mesh->GetMaterial());
		for (int i = 0; i < textures.size(); i++)
		{
			textures[i]->Bind(i);
		}

		InternalMeshAccessor::Bind(mesh);
		RenderCommand::DrawIndexed(mesh);
	}
}