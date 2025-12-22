#pragma once

#include <memory>
#include "Core/Base.h"
#include "Window/Window.h"
#include "LayerStack.h"
#include "Event/KeyboardEvent.h"
#include "Input/Input.h"

namespace Engine {

	class Application
	{
	public:

		ENGINE_API Application();
		ENGINE_API virtual ~Application();

		ENGINE_API void virtual Initialize();

		ENGINE_API void TickInternal();
		ENGINE_API void virtual Tick();

		ENGINE_API void PushLayer(Layer* layer);
		ENGINE_API void PushOverlay(Layer* overlay);

	private:		
		LayerStack layerStack;
	};

}

extern std::unique_ptr<Engine::Application> CreateApplication();
