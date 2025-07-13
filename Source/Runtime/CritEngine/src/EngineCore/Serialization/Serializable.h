#pragma once
#include <memory>

namespace Engine {

	struct ISerializable
	{
	public:

		virtual void Serialize(std::ostream& write) = 0;
		virtual void Deserialize(std::istream& read) = 0;
	};

}