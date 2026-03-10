#include "MainLoop.h"
#include "GlobalEngine.h"

namespace Engine
{
    MainLoop::MainLoop()
        : m_is_running(false)
    {
    };

    void MainLoop::run()
    {
        logInfo("MainLoop", "Starting");
        this->m_is_running = true;

        while (m_is_running)
        {
            this->tick();
        }
    };

    void MainLoop::stop()
    {
        this->m_is_running = false;
    };

    void MainLoop::tick()
    {
        GlobalEngine::get().tick();
    };
}
