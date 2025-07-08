#pragma once
#include "../Core/Base.h"
#include "../Core/BaseObject.h"
#include "../Core/Reflection/Base.h" //Move this to an intermediate object class
#include "DefaultSceneGraphable.h"
#include "../Serialization/DefaultSerializable.h" //Move this to an intermediate object class

namespace Engine {

	class Actor : public BaseObject, public DefaultSceneGraphable, public Reflectable, public DefaultSerializable
	{
	public:
		REFLECT(Actor);

		ENGINE_API Actor();

		
		int health = 10;
		float mana = 0;

		int GetInteger(std::string str) { 
			std::cout << "Hello, from GetInteger!! " << str << "\n";
			return 10; 
		};
	};

	PROPERTY(Actor, health);
	PROPERTY(Actor, mana);

	METHOD(Actor, GetInteger);

}