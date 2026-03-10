#pragma once

#include "Core/Base.h"
#include "Layer.h"
#include <vector>

namespace Engine
{
    class LayerStack
    {
    public:
        ENGINE_API LayerStack();
        ENGINE_API ~LayerStack();

        ENGINE_API void pushLayer(Layer* layer);
        ENGINE_API void popLayer(Layer* layer);
        ENGINE_API void pushOverlay(Layer* overlay);
        ENGINE_API void popOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_layers.end(); }

    private:
        std::vector<Layer*> m_layers;
        std::vector<Layer*>::iterator m_layerInsert;
    };
}
