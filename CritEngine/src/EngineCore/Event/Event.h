#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "../Core/Base.h"

namespace Engine {

	class EventBase
	{
	public:
		virtual ~EventBase() = default;
	};

	template<typename... Ts>
	class Event : public EventBase
	{
	public:

		void AddListener(std::function<void(Ts...)> func);
		void RemoveListener(std::function<void(Ts...)> func);
		void Emit(Ts... args);
	private:
		std::vector<std::function<void(Ts...)>> callbacks;
	};

	template <typename... Ts>
	inline void Event<Ts...>::Emit(Ts... args)
	{
		for (int32_t i = 0; i < this->callbacks.size(); i++)
		{
			this->callbacks[i](std::forward<Ts>(args)...);
		}
	}

	template<>
	inline void Event<>::Emit()
	{
		for (int32_t i = 0; i < this->callbacks.size(); i++)
		{
			this->callbacks[i]();
		}
	}

	template <typename... Ts>
	inline void Event<Ts...>::AddListener(std::function<void(Ts...)> func)
	{
		this->callbacks.push_back(func);
	}

	template<>
	inline void Event<>::AddListener(std::function<void()> func)
	{
		this->callbacks.push_back(func);
	}

	template <typename... Ts>
	inline void Event<Ts...>::RemoveListener(std::function<void(Ts...)> func)
	{
		uint32_t i = 0;
		for (std::function<void(Ts...)> callback : this->callbacks)
		{

			if (func.target<void(Ts...)>() == callback.target<void(Ts...)>())
			{
				this->callbacks.at(i) = this->callbacks.back();
				this->callbacks.pop_back();
			}
			i++;
		}
	}
	
	template <>
	inline void Event<>::RemoveListener(std::function<void()> func)
	{
		uint32_t i = 0;
		for (std::function<void()> callback : this->callbacks)
		{

			if (func.target<void()>() == callback.target<void()>())
			{
				this->callbacks.at(i) = this->callbacks.back();
				this->callbacks.pop_back();
			}
			i++;
		}
	}
	

};
