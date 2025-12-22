#pragma once

#include "Window.h"

namespace Engine {

	class WindowManager
	{
	public:
		WindowManager();
		~WindowManager();

		void Tick();
		
		ENGINE_API std::weak_ptr<Window> CreateWindow(const int width, const int height, const std::string& title = "CritEngine");
		ENGINE_API bool DestroyWindow(std::weak_ptr<Window> window);
		ENGINE_API bool DestroyWindow(Window* window);
		

	private:
		std::vector<std::shared_ptr<Window>> windows;
	};

}