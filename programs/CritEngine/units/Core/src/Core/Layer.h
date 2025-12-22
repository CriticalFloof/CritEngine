#pragma once

#include "Event/Event.h"

namespace Engine {

	class Layer {

	public:
		ENGINE_API Layer(const std::string& name = "UnnamedLayer");
		virtual ENGINE_API ~Layer(); 

		//virtual ENGINE_API void OnEvent(Event& event) = 0;
		virtual ENGINE_API void OnAttach() {}
		virtual ENGINE_API void OnDetach() {}
		virtual ENGINE_API void OnUpdate() {}

	};
}
