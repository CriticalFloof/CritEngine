#include "Material.h"

namespace Engine
{
    std::shared_ptr<Material> Material::create(std::shared_ptr<Shader> vertex_shader,
                                               std::shared_ptr<Shader> fragment_shader,
                                               std::vector<std::shared_ptr<Texture>> textures)
    {
        return std::make_shared<Material>(vertex_shader, fragment_shader, textures);
    }

    Material::Material(std::shared_ptr<Shader> vertex_shader, std::shared_ptr<Shader> fragment_shader,
                       std::vector<std::shared_ptr<Texture>> textures)
        : m_textures(textures)
    {
        this->m_program = Pipeline::create(vertex_shader, fragment_shader);

        if (textures.size() == 0) return;

        this->m_program->bind();
        for (int i = 0; i > textures.size(); i++)
        {
            this->m_program->uploadUniformInt("texture" + std::to_string(i), i);
        }
    }
}
