#pragma once

#include "Event.h"
#include <unordered_map>
#include <utility>

namespace Engine {
	
	class EventBus
	{

	public:

		template <typename... Ts>
		void AddEvent(const std::string& eventName, std::shared_ptr<Event<Ts...>> newEvent)
		{
			ASSERT(!this->DoesEventExist(eventName), "Cannot overwrite registered event!");
			this->events.insert({ eventName, newEvent });
		}

		template <>
		void AddEvent(const std::string& eventName, std::shared_ptr<Event<>> newEvent)
		{
			ASSERT(!this->DoesEventExist(eventName), "Cannot overwrite registered event!");
			this->events.insert({ eventName, newEvent });
		}

		void RemoveEvent(const std::string& eventName)
		{
			ASSERT(this->DoesEventExist(eventName), "Event doesn't exist on EventBus!");
			this->events.erase(eventName);
		}

		template <typename... Ts>
		void AddListener(const std::string& eventName, std::function<void(Ts...)> func)
		{
			ASSERT(this->DoesEventExist(eventName), "Event doesn't exist on EventBus!");
			std::shared_ptr<Event<Ts...>> storedEvent = std::dynamic_pointer_cast<Event<Ts...>>(this->GetEvent(eventName));
			storedEvent->AddListener(func);
		}

		template <typename... Ts>
		void RemoveListener(const std::string& eventName, std::function<void(Ts...)> func)
		{
			ASSERT(this->DoesEventExist(eventName), "Event doesn't exist on EventBus!");
			std::shared_ptr<Event<Ts...>> storedEvent = std::dynamic_pointer_cast<Event<Ts...>>(this->GetEvent(eventName));
			storedEvent->RemoveListener(func);
		}

		template <typename... Ts>
		void Emit(const std::string& eventName, Ts... args)
		{
			ASSERT(this->DoesEventExist(eventName), "Event doesn't exist on EventBus!");
			std::shared_ptr<Event<Ts...>> storedEvent = std::dynamic_pointer_cast<Event<Ts...>>(this->GetEvent(eventName));
			storedEvent->Emit(args...);
		}

		void RemoveAllListeners(const std::string& eventName)
		{
			ASSERT(this->DoesEventExist(eventName), "Event doesn't exist on EventBus!");
			this->events[eventName] = 0;
		}

		void RemoveAllEmitters()
		{
			this->events.clear();
		}


	private:

		inline std::shared_ptr<EventBase> GetEvent(const std::string& eventName) {  
			if (auto search = this->events.find(eventName); search != this->events.end())
				return search->second;
			else
				return nullptr;
		}
		inline bool DoesEventExist(const std::string& eventName)
		{
			if (this->events.find(eventName) != this->events.end())
				return true;
			else
				return false;
		}

		std::unordered_map<std::string, std::shared_ptr<EventBase>> events;
	};
}

