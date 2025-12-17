#pragma once
#include "../Core/Base.h"
#include "../Core/BaseObject.h"
#include "DefaultSceneGraphable.h"
#include <refl.hpp>

namespace Engine {

	class Actor : public BaseObject, public DefaultSceneGraphable
	{
	public:
		ENGINE_API Actor();
		virtual ~Actor() {};

		
		int health = 10;
		char mana = 60;

		int GetInteger(std::string str) { 
			std::cout << "Hello, from GetInteger!! " << str << "\n";
			return 10; 
		};
	};
}

REFL_TYPE(Engine::Actor, bases<>)
    REFL_FIELD(health, refl::attr::usage::member())
	REFL_FIELD(mana, refl::attr::usage::member())
	REFL_FUNC(GetInteger)
REFL_END