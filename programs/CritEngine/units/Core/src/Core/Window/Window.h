#pragma once

#include "../Event/Event.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/PIL/RenderContext.h"
#include "../Input/Input.h"

struct GLFWwindow;

namespace Engine {

	class Window
	{
	public:

		ENGINE_API Window(const int width, const int height, const std::string& title);
		ENGINE_API ~Window();

		ENGINE_API GLFWwindow* GetHandle();
		ENGINE_API std::shared_ptr<InputListener> GetInput();

		ENGINE_API void SetWidth(const int _width) { this->width = _width; }
		ENGINE_API void SetHeight(const int _height) { this->height = _height; }

		ENGINE_API int GetWidth() { return this->width; }
		ENGINE_API int GetHeight() { return this->height; }
		ENGINE_API float GetAspectRatio() { return (float)this->width / (float)this->height; }

		ENGINE_API void ImGuiStartFrame();
		ENGINE_API void ImGuiRender();

		void Tick();

	private:
		void PollEvents();
		void SwapBuffers();

		EventEmitter eventEmitter;
		GLFWwindow* windowHandle = nullptr;
		std::shared_ptr<InputListener> input = nullptr;
		std::shared_ptr<RenderContext> renderContext = nullptr;
		int width;
		int height;
		
	};

	struct WindowEvent : Event<std::tuple<>, AnyEvent> {};
	struct WindowResizeEvent : Event<std::tuple<Window*, int, int>, WindowEvent> {};
	struct WindowCloseEvent : Event<std::tuple<Window*>, WindowEvent> {};

};