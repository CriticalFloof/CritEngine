#include "LayerStack.h"

namespace Engine {

	LayerStack::LayerStack()
	{
		layerInsert = layers.begin(); // Iterator points to the beginning of the vector
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : layers) {
			delete layer;
		}
	}

	// Our layers are placed in the top half of the stack
	// The order of our layers determine what gets drawn first
	// We want to ensure layers don't ever get drawn after overlays

	void LayerStack::PushLayer(Layer* layer)
	{
		layerInsert = layers.emplace(layerInsert, layer); // Inserts the element to the top of the vector
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end()) {
			layers.erase(it);
			layerInsert--;
		}
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		layers.emplace_back(overlay); // Overlays are placed in the bottom half of the stack, because overlays are rendered last
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if (it != layers.end()) {
			layers.erase(it);
		}
	}

}