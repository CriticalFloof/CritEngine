#include "GraphicsFeature.h"

namespace Engine
{
    GraphicsFeature::GraphicsFeature(GraphicsFeatureSupport support_level)
        : m_supportLevel(support_level)
    {
    };

    GraphicsFeatureSupport GraphicsFeature::getSupportLevel()
    {
        return this->m_supportLevel;
    };
}
