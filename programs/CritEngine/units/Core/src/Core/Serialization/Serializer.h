#pragma once

#include <refl.hpp>
#include <iostream>
#include <type_traits>

namespace Serialization {

	template <typename T>
	concept Reflectable = refl::trait::is_reflectable_v<T>; 

	template<typename T>
	concept Formattable = requires(T a)
	{
		std::cin >> a;
		std::cout << a;
	};

	struct Node
	{
		struct Field
		{
			std::string name;
			std::string value;
		};
		std::vector<Field> fields;
	};


	template <Reflectable T>
	void Serialize(T&& object, std::ostream& out)
	{
		refl::util::for_each(refl::reflect(object).members, [&](auto member)
		{
			if constexpr (refl::descriptor::is_readable(member))
			{
				using MT = decltype(member)::value_type;

				if constexpr (Formattable<MT> && refl::descriptor::has_attribute<refl::attr::usage::member>(member))
				{
					out << refl::descriptor::get_display_name(member) << " = ";
					out << refl::descriptor::invoke(member, object) << "\n";
				}
				else if constexpr (Reflectable<MT> && !std::is_pointer_v<T>)
				{
					out << refl::descriptor::get_display_name(member) << "{\n";
					Serialize(refl::descriptor::invoke(member, object), out);
					out << "}\n";
				}
				else if constexpr (Reflectable<MT> && std::is_pointer_v<T>)
				{
					//out << refl::descriptor::get_display_name(member) << "\n";
				}
				else
				{
					static_assert(false, "Couldn't serialize type into property");
				}
			}
		});
	}
	
	template <Reflectable T>
	T Deserialize(std::istream& in)
	{

		std::string identifier;

		T instance {};
		refl::util::for_each(refl::reflect<T>().members, [&](auto member)
		{
			if constexpr (refl::descriptor::is_readable(member))
			{
				using MT = decltype(member)::value_type;

				// Deserialize Fundamentals Types
				if constexpr (Formattable<MT> && refl::descriptor::has_attribute<refl::attr::usage::member>(member))
				{
					MT value;

					in >> identifier;
					in.ignore(3); // " = "
					in >> value;
					refl::descriptor::invoke(member, instance, value);
				}
				// Deserialize Compound Types
				else if constexpr (Reflectable<MT> && !std::is_pointer_v<T>)
				{
					in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					refl::descriptor::invoke(member, instance, Deserialize<MT>(in));
					in.ignore(2); // "}\n"
				}
				// Resolve other objects
				else if constexpr (Reflectable<MT> && std::is_pointer_v<T>)
				{

				}
				else
				{
					static_assert(false, "Couldn't deserialize type into property");
				}
			}
		});

		return instance;
	}
}

