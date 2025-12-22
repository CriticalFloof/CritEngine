#include "WindowManager.h"
#include <algorithm>

namespace Engine {

	WindowManager::WindowManager()
	{
		this->windows.reserve(4);
	}

	WindowManager::~WindowManager()
	{
	}

	void WindowManager::Tick()
	{
		for (int i = 0; i < this->windows.size(); i++)
		{
			this->windows[i]->Tick();
		}
	}

	std::weak_ptr<Window> WindowManager::CreateWindow(const int width, const int height, const std::string& title)
	{
		this->windows.push_back(std::make_shared<Window>(width, height, title));
		return std::weak_ptr<Window>(this->windows.back());
	}

	bool WindowManager::DestroyWindow(std::weak_ptr<Window> window)
	{
		std::shared_ptr<Window> reference_window = window.lock();
		if (reference_window == nullptr) return false;

		auto result = std::find_if(
			this->windows.begin(),
			this->windows.end(),
			[&reference_window](const std::shared_ptr<Window>& stored_window) { 
			    return reference_window.get() == stored_window.get();
		    }
		);
		if (result == this->windows.end()) return false;

		std::iter_swap(result, this->windows.end() - 1);
		this->windows.pop_back();

		return true;
	}

	ENGINE_API bool WindowManager::DestroyWindow(Window* window)
	{
		std::vector<std::shared_ptr<Window>>::iterator result = std::find_if(
			this->windows.begin(),
			this->windows.end(),
			[&window](const std::shared_ptr<Window>& stored_window)
		{
			return stored_window.get() == window;
		});

		if (result == this->windows.end()) return false;

		std::iter_swap(result, this->windows.end() - 1);
		this->windows.pop_back();

		return true;
	}

}