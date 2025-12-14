#pragma once
#include "../Core/Base.h"
#include "../Core/BaseObject.h"
#include "DefaultSceneGraphable.h"
#include "../Serialization/DefaultSerializable.h" //Move this to an intermediate object class

namespace Engine {

	class Actor : public BaseObject, public DefaultSceneGraphable
	{
	public:
		ENGINE_API Actor();

		
		int health = 10;
		char mana = 60;

		int GetInteger(std::string str) { 
			std::cout << "Hello, from GetInteger!! " << str << "\n";
			return 10; 
		};
	};
}