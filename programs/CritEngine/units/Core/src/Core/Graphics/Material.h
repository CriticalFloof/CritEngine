#pragma once

#include "PIL/Pipeline.h"
#include "PIL/Texture.h"

namespace Engine {

	class InternalMaterialAccessor;

	class Material
	{
	public:
		friend class InternalMaterialAccessor;

		Material(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader, std::vector<std::shared_ptr<Texture>> textures);

		ENGINE_API static std::shared_ptr<Material> Create(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader, std::vector<std::shared_ptr<Texture>> textures);
	private:
		const std::shared_ptr<Pipeline> GetPipeline() const { return this->program; }
		const std::vector<std::shared_ptr<Texture>> GetTextures() const { return this->textures; };

		std::shared_ptr<Pipeline> program;
		std::vector<std::shared_ptr<Texture>> textures;
	};

	class InternalMaterialAccessor
	{
	public:
		static std::shared_ptr<Pipeline> GetPipeline(const std::shared_ptr<Material>& material)
		{
			return material->GetPipeline();
		}

		static std::vector<std::shared_ptr<Texture>> GetTextures(const std::shared_ptr<Material>& material)
		{
			return material->GetTextures();
		}
	};
}