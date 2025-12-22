#include "Material.h"

namespace Engine {

	std::shared_ptr<Material> Material::Create(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader, std::vector<std::shared_ptr<Texture>> textures)
	{
		return std::make_shared<Material>(vertexShader, fragmentShader, textures);
	}

	Material::Material(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader, std::vector<std::shared_ptr<Texture>> textures)
		: textures(textures)
	{
		this->program = Pipeline::Create(vertexShader, fragmentShader);

		if (textures.size() == 0) return;

		this->program->Bind();
		for (int i = 0; i > textures.size(); i++)
		{
			this->program->UploadUniformInt("texture" + std::to_string(i), i);
		}
	}

}