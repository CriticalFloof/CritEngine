#pragma once 
#include <string>
#include <cstdio>

namespace Reflection {

	struct TypeInfo
	{
		template<typename T>
		static TypeInfo Create(std::string name) 
		{
			TypeInfo t = TypeInfo();
			t.name = name;
			t.size = sizeof(T);
			t.alignment = alignof(T);
			t.kind = Kind::Unknown;

			std::cout << "Created Metadata " + std::to_string((long)&t) << std::endl;

			return t;
		}

		~TypeInfo()
		{
			std::cout << "Destroyed Metadata " << std::endl;
		}

		std::string name;
		std::size_t size;
		std::size_t alignment;

		enum class Kind : uint8_t
		{
			Integral,
			FloatingPoint,
			Boolean,
			Character,
			Unknown
		} kind;
	};
	
}