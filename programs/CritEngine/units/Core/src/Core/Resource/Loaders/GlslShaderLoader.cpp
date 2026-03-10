#include "GlslShaderLoader.h"
#include "../../Logging/Logger.h"
#include "../../Profiler/Profiler.h"

namespace Engine
{
    std::string GlslShaderLoader::getResourceType()
    {
        return "Shader";
    }

    std::shared_ptr<void> GlslShaderLoader::load(std::filesystem::path filepath)
    {
        CE_PROFILE_FUNC(Load_GLSLShader);

        FileAccessor file;
        file.openFile(filepath);

        if (file.getSize() <= 0)
        {
            logError("GLSLShaderLoader", "Failed to open file.");
            return nullptr;
        }

        this->m_shaderFile.resize(file.getSize());
        file.readBuffer(reinterpret_cast<uint8_t*>(this->m_shaderFile.data()), file.getSize());

        std::string ext = filepath.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

        if (ext == VERTEX_SHADER_EXT)
        {
            this->m_shaderType = ShaderType::Vertex;
        }
        else if (ext == FRAGMENT_SHADER_EXT)
        {
            this->m_shaderType = ShaderType::Fragment;
        }
        else if (ext == GEOMETRY_SHADER_EXT)
        {
            this->m_shaderType = ShaderType::Geometry;
        }
        else if (ext == MESH_SHADER_EXT)
        {
            this->m_shaderType = ShaderType::Mesh;
        }
        else if (ext == TASK_SHADER_EXT)
        {
            this->m_shaderType = ShaderType::Task;
        }
        else if (ext == TESC_SHADER_EXT)
        {
            this->m_shaderType = ShaderType::TessellationControl;
        }
        else if (ext == TESE_SHADER_EXT)
        {
            this->m_shaderType = ShaderType::TessellationEvaluation;
        }
        else if (ext == COMPUTE_SHADER_EXT)
        {
            this->m_shaderType = ShaderType::Compute;
        }
        else
        {
            logError("GLSLShaderLoader", "Unsupported shader extension '" + ext + "'!");
            return nullptr;
        }

        return Shader::create(this->m_shaderFile, this->m_shaderType);
    }
}
