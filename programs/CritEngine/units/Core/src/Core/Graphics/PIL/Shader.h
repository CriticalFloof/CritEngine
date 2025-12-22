#pragma once
#include <string>
#include "../../Core/Base.h"
#include "../../Math/Common.h"


namespace Engine {

	enum class ShaderType : uint8_t
	{
		Vertex,
		Fragment,
		Geometry,
		Mesh,
		Task,
		TessellationControl,
		TessellationEvaluation,
		Compute
	};

	class Shader
	{
	public:
		ENGINE_API static std::shared_ptr<Shader> Create(const std::string& glslSource, const ShaderType shaderType);

		virtual ~Shader() {};
	};

}