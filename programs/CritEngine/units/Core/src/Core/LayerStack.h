#pragma once

#include "Core/Base.h"
#include "Layer.h"
#include <vector>

namespace Engine {

	class LayerStack {

	public:
		ENGINE_API LayerStack();
		ENGINE_API ~LayerStack();

		ENGINE_API void PushLayer(Layer* layer);
		ENGINE_API void PopLayer(Layer* layer);
		ENGINE_API void PushOverlay(Layer* overlay);
		ENGINE_API void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }
		
	private:
		std::vector<Layer*> layers;
		std::vector<Layer*>::iterator layerInsert;
		
	};

}