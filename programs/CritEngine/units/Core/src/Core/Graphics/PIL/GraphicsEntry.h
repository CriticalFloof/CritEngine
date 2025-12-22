#pragma once

#include "../../Core/Base.h"

namespace Engine {

	class GraphicsEntry
	{
	public:
		virtual ~GraphicsEntry() = 0;

		static std::unique_ptr<GraphicsEntry> Create();

		virtual void Init() = 0;

	};

}