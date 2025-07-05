#pragma once
#include "../Core/Base.h"
#include "../Core/BaseObject.h"
#include "../Core/Reflection.h"
#include "DefaultSceneGraphable.h"

namespace Engine {

	class Actor : public BaseObject, public DefaultSceneGraphable
	{
	public:
		REFLECT(Actor);

		ENGINE_API Actor();

		
		int health = 0;
		float mana = 0;
	};

	PROPERTY(Actor, int, health);
	PROPERTY(Actor, float, mana);
}