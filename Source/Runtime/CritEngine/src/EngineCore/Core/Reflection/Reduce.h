#pragma once

namespace Reflection {

	template<typename T>
	struct reduce
	{
		using type = T;
	};

	template<typename T>
	struct reduce<T&>
	{
		using type = typename reduce<T>::type;
	};

	template<typename T>
	struct reduce<T&&>
	{
		using type = typename reduce<T>::type;
	};

	template<typename T>
	struct reduce<T*>
	{
		using type = typename reduce<T>::type;
	};

	template<typename T>
	struct reduce<T* const>
	{
		using type = typename reduce<T>::type;
	};

	template<typename T>
	struct reduce<T* volatile>
	{
		using type = typename reduce<T>::type;
	};

	template<typename T>
	struct reduce<T* const volatile>
	{
		using type = typename reduce<T>::type;
	};

	template<typename T>
	struct reduce<const T>
	{
		using type = typename reduce<T>::type;
	};

	template<typename T>
	struct reduce<volatile T>
	{
		using type = typename reduce<T>::type;
	};

	template<typename T>
	struct reduce<const volatile T>
	{
		using type = typename reduce<T>::type;
	};


}