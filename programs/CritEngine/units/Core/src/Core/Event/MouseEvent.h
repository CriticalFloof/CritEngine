#pragma once

#include "../Logging/Logger.h"
#include "Event.h"

namespace Engine {

	struct MouseEvent : Event<std::tuple<>, AnyEvent> {};

	struct MouseButtonPressedEvent : Event<std::tuple<unsigned int>, MouseEvent> {};
	struct MouseButtonReleasedEvent : Event<std::tuple<unsigned int>, MouseEvent> {};
	struct MouseButtonJustPressedEvent : Event<std::tuple<unsigned int>, MouseEvent> {};

	struct MouseMoveEvent : Event<std::tuple<Vector2>, MouseEvent> {};
}