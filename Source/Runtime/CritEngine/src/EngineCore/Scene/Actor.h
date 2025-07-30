#pragma once
#include "../Core/Base.h"
#include "../Core/BaseObject.h"
#include "../Core/Reflection/Common.h"
#include "DefaultSceneGraphable.h"
#include "../Serialization/DefaultSerializable.h" //Move this to an intermediate object class

namespace Engine {

	class Actor : public BaseObject, public DefaultSceneGraphable, public DefaultSerializable
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

#ifdef ENGINE_BUILD_DLL
	namespace {

		::Reflection::KindInfo kind = ::Reflection::KindInfo::Create<Actor>("Actor")
			.AddClassMember("health", &Actor::health)
			.AddClassMember("mana", &Actor::mana);
		REFLECT_KIND(Actor, kind);
	}
#endif
}