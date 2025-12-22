#pragma once

#include "../ResourceLoader.h"
#include "../FileAccessor.h"
#include "../../Graphics/PIL/Shader.h"

namespace Engine {

    inline constexpr std::string_view VERTEX_SHADER_EXT = ".vertshader";
    inline constexpr std::string_view FRAGMENT_SHADER_EXT = ".fragshader";
    inline constexpr std::string_view GEOMETRY_SHADER_EXT = ".geomshader";
    inline constexpr std::string_view MESH_SHADER_EXT = ".meshshader";
    inline constexpr std::string_view TASK_SHADER_EXT = ".taskshader";
    inline constexpr std::string_view TESC_SHADER_EXT = ".tescshader";
    inline constexpr std::string_view TESE_SHADER_EXT = ".teseshader";
    inline constexpr std::string_view COMPUTE_SHADER_EXT = ".compshader";

class GLSLShaderLoader : public ResourceLoader {
public:

	virtual ~GLSLShaderLoader() override {};

	virtual std::string GetResourceType() override;
	virtual std::shared_ptr<void> Load(std::filesystem::path filepath) override;

private:
	std::string shaderFile;
	Engine::ShaderType shaderType;

};

}
