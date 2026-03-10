#pragma once

#include <array>
#include <unordered_map>
#include "GraphicsFeature.h"

namespace Engine
{
    enum GraphicsFeatureType : uint8_t
    {
        PipelineDynamicStateViewport,
        PipelineStaticStateViewport,
        PipelineDynamicStateScissor,
        PipelineStaticStateScissor,
        PipelineDynamicStateLineWidth,
        PipelineStaticStateLineWidth,
        PipelineDynamicStateDepthBias,
        PipelineStaticStateDepthBias,
        PipelineDynamicStateBlendConstants,
        PipelineStaticStateBlendConstants,
        PipelineDynamicStateDepthBounds,
        PipelineStaticStateDepthBounds,
        PipelineDynamicStateStencilCompareMask,
        PipelineStaticStateStencilCompareMask,
        PipelineDynamicStateStencilWriteMask,
        PipelineStaticStateStencilWriteMask,
        PipelineDynamicStateStencilReference,
        PipelineStaticStateStencilReference,
        Size
    };

    class GraphicsFeatureRegistry
    {
    public:
        GraphicsFeatureRegistry(std::unordered_map<GraphicsFeatureType, GraphicsFeature> features);

        GraphicsFeatureSupport getFeatureSupport(GraphicsFeatureType type);

    private:
        std::array<GraphicsFeature, Size> m_features;
    };
}
