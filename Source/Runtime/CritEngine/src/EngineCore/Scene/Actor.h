#pragma once
#include "../Core/Base.h"
#include "../Core/BaseObject.h"
#include "DefaultSceneGraphable.h"

namespace Engine {

	class Actor : public BaseObject, public DefaultSceneGraphable
	{
	public:
		ENGINE_API Actor();
	};

}