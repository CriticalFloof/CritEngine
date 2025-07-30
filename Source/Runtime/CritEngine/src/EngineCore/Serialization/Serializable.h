#pragma once
#include <memory>

namespace Engine {

	struct ISerializable
	{
	public:

		virtual bool Serialize(std::ostream& write) = 0;
		virtual bool Deserialize(std::istream& read) = 0;
	};

}