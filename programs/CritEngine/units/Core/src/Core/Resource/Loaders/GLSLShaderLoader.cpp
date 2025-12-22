#include "GLSLShaderLoader.h"
#include "../../Logging/Logger.h"
#include "../../Profiler/Profiler.h"

namespace Engine {

    std::string GLSLShaderLoader::GetResourceType()
    {
        return "Shader";
    }

    std::shared_ptr<void> GLSLShaderLoader::Load(std::filesystem::path filepath) {
        CE_PROFILE_FUNC(Load_GLSLShader);

        FileAccessor file;
        file.OpenFile(filepath);

        if (file.GetSize() <= 0) {
            LogError("GLSLShaderLoader", "Failed to open file.");
            return nullptr;
        }

        this->shaderFile.resize(file.GetSize());
        file.ReadBuffer(reinterpret_cast<uint8_t*>(this->shaderFile.data()), file.GetSize());

        std::string ext = filepath.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ext == VERTEX_SHADER_EXT) {
            this->shaderType = Engine::ShaderType::Vertex;
        }
        else if (ext == FRAGMENT_SHADER_EXT) {
            this->shaderType = Engine::ShaderType::Fragment;
        }
        else if (ext == GEOMETRY_SHADER_EXT) {
            this->shaderType = Engine::ShaderType::Geometry;
        }
        else if (ext == MESH_SHADER_EXT) {
            this->shaderType = Engine::ShaderType::Mesh;
        }
        else if (ext == TASK_SHADER_EXT) {
            this->shaderType = Engine::ShaderType::Task;
        }
        else if (ext == TESC_SHADER_EXT) {
            this->shaderType = Engine::ShaderType::TessellationControl;
        }
        else if (ext == TESE_SHADER_EXT) {
            this->shaderType = Engine::ShaderType::TessellationEvaluation;
        }
        else if (ext == COMPUTE_SHADER_EXT) {
            this->shaderType = Engine::ShaderType::Compute;
        }
        else {
            LogError("GLSLShaderLoader", "Unsupported shader extension '" + ext + "'!");
            return nullptr;
        }

        return Shader::Create(this->shaderFile, this->shaderType);
    }

}