#pragma once
#include <string>

namespace Engine {

	struct ISerializable
	{
	public:

		virtual std::string Serialize() = 0;
		virtual void Deserialize(std::string source) = 0;
	};

}