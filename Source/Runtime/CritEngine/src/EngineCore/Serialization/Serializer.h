#pragma once

#include <refl.hpp>



template <typename T>
std::unique_ptr<void> Serialize(T&& object)
{
	std::unique_ptr<void> result;

	// iterate over the members of T
	for_each(refl::reflect(value).members, [&](auto member)
	{
		// is_readable checks if the member is a non-const field
		// or a 0-arg const-qualified function marked with property attribute
		if constexpr (is_readable(member) && refl::descriptor::has_attribute<refl::attr::usage::member>(member))
		{
			// get_display_name prefers the friendly_name of the property over the function name
			os << get_display_name(member) << "=";
			// member(value) returns a reference to the field or calls the property accessor
			os << member(value) << ";";
		}
	});
}

template <typename T>
std::unique_ptr<T> Deserialize(std::unique_ptr<void> json)
{
	//TBD
}