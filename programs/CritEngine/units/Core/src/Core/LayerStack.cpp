#include "LayerStack.h"

namespace Engine
{
    LayerStack::LayerStack()
    {
        m_layerInsert = m_layers.begin(); // Iterator points to the beginning of the vector
    }

    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_layers)
        {
            delete layer;
        }
    }

    // Our layers are placed in the top half of the stack
    // The order of our layers determine what gets drawn first
    // We want to ensure layers don't ever get drawn after overlays

    void LayerStack::pushLayer(Layer* layer)
    {
        m_layerInsert = m_layers.emplace(m_layerInsert, layer); // Inserts the element to the top of the vector
    }

    void LayerStack::popLayer(Layer* layer)
    {
        auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        if (it != m_layers.end())
        {
            m_layers.erase(it);
            --m_layerInsert;
        }
    }

    void LayerStack::pushOverlay(Layer* overlay)
    {
        m_layers.emplace_back(overlay);
        // Overlays are placed in the bottom half of the stack, because overlays are rendered last
    }

    void LayerStack::popOverlay(Layer* overlay)
    {
        auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
        if (it != m_layers.end())
        {
            m_layers.erase(it);
        }
    }
}
