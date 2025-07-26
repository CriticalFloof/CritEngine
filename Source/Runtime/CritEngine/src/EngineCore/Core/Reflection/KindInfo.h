#pragma once 
#include <string>
#include <cstdio>
#include <type_traits>

namespace Reflection {

	struct KindInfo
	{
		template<typename T>
		static KindInfo Create(std::string name)
		{
			KindInfo t = KindInfo();
			t.name = name;
			t.size = sizeof(T);
			t.alignment = alignof(T);
			return t;
		}

		template<>
		static KindInfo Create<void>(std::string name)
		{
			KindInfo t = KindInfo();
			t.name = name;
			t.size = 0;
			t.alignment = 0;
			return t;
		}

		std::string name;
		std::size_t size;
		std::size_t alignment;
	};
	
}