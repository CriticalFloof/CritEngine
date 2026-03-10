#pragma once

#include <stdint.h>

namespace Engine
{
    enum GraphicsFeatureSupport : uint8_t
    {
        Unsupported,
        Emulated,
        Available
    };

    class GraphicsFeature
    {
    public:
        GraphicsFeature(GraphicsFeatureSupport support_level = Unsupported);

        GraphicsFeatureSupport getSupportLevel();

    private:
        GraphicsFeatureSupport m_supportLevel;
    };
}
