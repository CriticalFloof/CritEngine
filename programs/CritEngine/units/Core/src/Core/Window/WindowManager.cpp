#include "WindowManager.h"
#include <algorithm>

namespace Engine
{
    WindowManager::WindowManager()
    {
        m_windows.reserve(4);
    }

    WindowManager::~WindowManager()
    {
    }

    void WindowManager::tick()
    {
        for (size_t i = 0; i < m_windows.size(); i++)
        {
            m_windows[i]->tick();
        }
    }

    std::weak_ptr<Window> WindowManager::createWindow(const int width, const int height, const std::string& title)
    {
        m_windows.push_back(std::make_shared<Window>(width, height, title));
        return std::weak_ptr<Window>(m_windows.back());
    }

    bool WindowManager::destroyWindow(std::weak_ptr<Window> window)
    {
        std::shared_ptr<Window> reference_window = window.lock();
        if (reference_window == nullptr) return false;

        auto result = std::find_if(
            m_windows.begin(),
            m_windows.end(),
            [&reference_window](const std::shared_ptr<Window>& stored_window)
            {
                return reference_window.get() == stored_window.get();
            }
        );
        if (result == m_windows.end()) return false;

        std::iter_swap(result, m_windows.end() - 1);
        m_windows.pop_back();

        return true;
    }

    ENGINE_API bool WindowManager::destroyWindow(Window* window)
    {
        auto result = std::find_if(
            m_windows.begin(),
            m_windows.end(),
            [&window](const std::shared_ptr<Window>& stored_window)
            {
                return stored_window.get() == window;
            });

        if (result == m_windows.end()) return false;

        std::iter_swap(result, m_windows.end() - 1);
        m_windows.pop_back();

        return true;
    }
}
