#include "GraphicsFeatureRegistry.h"
#include "../../../Core/Base.h"

namespace Engine
{
    GraphicsFeatureRegistry::GraphicsFeatureRegistry(
        std::unordered_map<GraphicsFeatureType, GraphicsFeature> feature_set)
    {
        ASSERT(feature_set.size() == GraphicsFeatureType::Size,
               "A Graphics Backend must provide support information for all features");
        ASSERT(!feature_set.contains(GraphicsFeatureType::Size),
               "A Graphics Backend supporting feature capability for the 'Size' Enum is ill-formed.");

        for (auto it = feature_set.begin(); it != feature_set.end(); ++it)
        {
            this->m_features[it->first] = it->second;
        }
    }

    GraphicsFeatureSupport GraphicsFeatureRegistry::getFeatureSupport(GraphicsFeatureType type)
    {
        return this->m_features[type].getSupportLevel();
    }
}
