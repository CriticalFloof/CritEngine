#pragma once 
#include <string>
#include <cstdio>
#include <type_traits>

namespace Reflection {

	class TypeInfo;

	enum KindCategories : uint8_t
	{
		Integral = 1 << 0,
		FloatingPoint = 1 << 1,
		Enum = 1 << 2,
		Class = 1 << 3,
		Function = 1 << 4,
	};
	inline KindCategories operator|(KindCategories lhs, KindCategories rhs)
	{
		return static_cast<KindCategories>(
			static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)
		);
	}
	inline KindCategories operator&(KindCategories lhs, KindCategories rhs)
	{
		return static_cast<KindCategories>(
			static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)
		);
	}

	struct KindInfo
	{
		template<typename T>
		static KindInfo Create(std::string name)
		{
			KindInfo t = KindInfo();
			t.name = name;
			t.size = sizeof(T);
			t.alignment = alignof(T);

			if constexpr (std::is_integral_v<T>)
			{
				t.categories = t.categories | KindCategories::Integral;
			}
			if constexpr (std::is_floating_point_v<T>)
			{
				t.categories = t.categories | KindCategories::FloatingPoint;
			}
			if constexpr (std::is_enum_v<T>)
			{
				t.categories = t.categories | KindCategories::Enum;
				// fill enum entries, needs a way for users to register enum names explicitly.

			}
			if constexpr (std::is_class_v<T>)
			{
				t.categories = t.categories | KindCategories::Class;
				// fill class members, needs a way for users to register class members.
			}
			if constexpr (std::is_function_v<T>)
			{
				t.categories = t.categories | KindCategories::Function;
				// fill function signature fields, needs a way for users to register functions/methods.
			}

			return t;
		}

		template<>
		static KindInfo Create<void>(std::string name)
		{
			KindInfo t = KindInfo();
			t.name = name;
			return t;
		}

		std::string name;
		std::size_t size = 0;
		std::size_t alignment = 0;
		KindCategories categories = static_cast<KindCategories>(0);

	};
	
}