#pragma once
#include <functional>

#include "../Core/Base.h"

namespace Engine
{
    // This essentially serves as a wrapper around std::function<void()>
    // It provide some semantic clarity, but more importantly, we can easily extend functionality..
    // with features like priorities and IDs in the future without needing to refactor as much

    class Task
    {
    public:
        Task() = default;
        Task(std::function<void()> function) : taskFunction(std::move(function)) {}

        void Execute() { if (taskFunction) taskFunction(); }
        void Execute() const { if (taskFunction) taskFunction(); }
 //     bool IsValid() const { return static_cast<bool>(taskFunction); }

    private:
        std::function<void()> taskFunction;
    };

}
