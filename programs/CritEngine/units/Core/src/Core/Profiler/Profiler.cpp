#include <chrono>

#include "../Logging/Logger.h"
#include "Profiler.h"

namespace Engine
{
    // This is a very simple profiler that's split into two classes that can either be stopped manually, or
    // print results when it leaves the scope of a function. I provided macros in the header file to make this
    // even easier to setup.

    void Profile::stop()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - m_startTime).count();
        Debug::log(this->m_funcName, duration, "ns");
    }

    ScopedProfile::~ScopedProfile()
    {
        this->stop();
    }
}
