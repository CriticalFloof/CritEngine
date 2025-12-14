#pragma once
#include "../Core/Base.h"
#include "../Core/BaseObject.h"
#include "DefaultSceneGraphable.h"

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