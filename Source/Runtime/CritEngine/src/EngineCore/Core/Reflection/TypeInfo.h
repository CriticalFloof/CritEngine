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
			return t;
		}

		template<>
		static TypeInfo Create<void>(std::string name)
		{
			TypeInfo t = TypeInfo();
			t.name = name;
			t.size = 0;
			t.alignment = 0;
			t.kind = Kind::Unknown;
			return t;
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