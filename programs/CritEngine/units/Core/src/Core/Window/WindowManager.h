#pragma once

#include "Window.h"

namespace Engine
{
    class WindowManager
    {
    public:
        WindowManager();
        ~WindowManager();

        void tick();

        ENGINE_API std::weak_ptr<Window> createWindow(int width, int height, const std::string& title = "CritEngine");
        ENGINE_API bool destroyWindow(std::weak_ptr<Window> window);
        ENGINE_API bool destroyWindow(Window* window);

    private:
        std::vector<std::shared_ptr<Window>> m_windows;
    };
}
