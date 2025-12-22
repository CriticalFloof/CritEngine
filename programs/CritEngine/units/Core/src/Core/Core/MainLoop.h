#pragma once

#include <memory>

#include "Base.h"
#include "../Logging/Logger.h"


namespace Engine
{
	class MainLoop
	{
	public:
		ENGINE_API MainLoop();
		ENGINE_API void Run();
		ENGINE_API void Tick();
		ENGINE_API void Stop();

	private:
		bool is_running;
	};
}



