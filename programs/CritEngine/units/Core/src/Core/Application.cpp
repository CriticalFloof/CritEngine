#include "Application.h"
#include "Logging/Logger.h"
#include "Core/GlobalEngine.h"

namespace Engine {

	Application::Application()
	{
	};

	Application::~Application()
	{
	};

	void Application::Initialize()
	{
		// This function gets called after the global engine has been initialized.
	};

	void Application::TickInternal()
	{
		for (Layer* layer : this->layerStack)
			layer->OnUpdate();
	}

	void Application::PushLayer(Layer* layer)
	{
		this->layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		this->layerStack.PushLayer(overlay);
	}

	void Application::Tick()
	{
		LogInfo("App", "Tick!");
	}

}

