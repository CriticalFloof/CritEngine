#pragma once

#include "../Logging/Logger.h"
#include "Event.h"

namespace Engine {

	struct KeyboardEvent : Event<std::tuple<unsigned int>, AnyEvent> {};

	struct KeyPressedEvent : Event<std::tuple<>, KeyboardEvent> {};
	struct KeyReleasedEvent : Event<std::tuple<>, KeyboardEvent> {};
	struct KeyJustPressedEvent : Event<std::tuple<>, KeyboardEvent> {};
}
