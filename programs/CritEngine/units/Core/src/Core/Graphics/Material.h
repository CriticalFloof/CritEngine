#pragma once

#include "PIL/Pipeline.h"
#include "PIL/Texture.h"

namespace Engine
{
    class InternalMaterialAccessor;

    class Material
    {
    public:
        friend class InternalMaterialAccessor;

        Material(std::shared_ptr<Shader> vertex_shader, std::shared_ptr<Shader> fragment_shader,
                 std::vector<std::shared_ptr<Texture>> textures);

        ENGINE_API static std::shared_ptr<Material> create(std::shared_ptr<Shader> vertex_shader,
                                                           std::shared_ptr<Shader> fragment_shader,
                                                           std::vector<std::shared_ptr<Texture>> textures);

    private:
        const std::shared_ptr<Pipeline> getPipeline() const { return this->m_program; }
        const std::vector<std::shared_ptr<Texture>> getTextures() const { return this->m_textures; }

        std::shared_ptr<Pipeline> m_program;
        std::vector<std::shared_ptr<Texture>> m_textures;
    };

    class InternalMaterialAccessor
    {
    public:
        static std::shared_ptr<Pipeline> getPipeline(const std::shared_ptr<Material>& material)
        {
            return material->getPipeline();
        }

        static std::vector<std::shared_ptr<Texture>> getTextures(const std::shared_ptr<Material>& material)
        {
            return material->getTextures();
        }
    };
}
