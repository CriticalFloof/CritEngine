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

		
		int health = 10;
		float mana = 0;

		int GetInteger() { 
			std::cout << "Hello, from GetInteger!!" << "\n";
			return 10; 
		};
	};

	PROPERTY(Actor, health);
	PROPERTY(Actor, mana);

	METHOD(Actor, GetInteger);

}