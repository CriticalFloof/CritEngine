#include "GraphicsFeatureRegistry.h"
#include "../../../Core/Base.h"

namespace Engine {

	GraphicsFeatureRegistry::GraphicsFeatureRegistry(std::unordered_map<GraphicsFeatureType, GraphicsFeature> featureSet)
	{
		ASSERT(featureSet.size() == GraphicsFeatureType::SIZE, "A Graphics Backend must provide support information for all features");
		ASSERT(featureSet.find(GraphicsFeatureType::SIZE) == featureSet.end(), "A Graphics Backend supporting feature capability for the 'Size' Enum is ill-formed.");

		for (auto it = featureSet.begin(); it != featureSet.end(); ++it)
		{
			this->features[it->first] = it->second;
		}
	}

	GraphicsFeatureSupport GraphicsFeatureRegistry::GetFeatureSupport(GraphicsFeatureType type)
	{
		return this->features[type].GetSupportLevel();
	}

}