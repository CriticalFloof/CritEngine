#pragma once

#include <refl.hpp>

template <typename T>
std::unique_ptr<void> Serialize(T&& object)
{
}

template <typename T>
std::unique_ptr<T> Deserialize(std::unique_ptr<void> json)
{
	//TBD
}