#include "GraphicsFeature.h"

namespace Engine {

	GraphicsFeature::GraphicsFeature(GraphicsFeatureSupport supportLevel)
		: supportLevel(supportLevel)
	{
	};

	GraphicsFeatureSupport GraphicsFeature::GetSupportLevel()
	{
		return this->supportLevel;
	};

}