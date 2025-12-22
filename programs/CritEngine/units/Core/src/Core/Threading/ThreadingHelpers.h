#pragma once
#include <mutex>
#include <type_traits>
#include <optional>

#include "../Core/Base.h"

ENGINE_API unsigned int GetThreadCountEstimate();

namespace Engine {

	// Recommended for use with more complex structures
	template<typename T>
	class ThreadSafeVariable {
	public:
		ThreadSafeVariable(T _value) : value(_value) {}

		T Get() const {
			std::lock_guard<std::mutex> lock(this->mutex);
			return value;
		}

		void Set(T newValue) {
			std::lock_guard<std::mutex> lock(this->mutex);
			this->value = newValue;
		}

		// Attempts a non-blocking read
		std::optional<T> TryGet() const {
			if (!mutex.try_lock()) {
				return std::nullopt;
			}
			std::lock_guard<std::mutex> lock(mutex, std::adopt_lock);
			return value;
		}

		// Modify the value atomically using a lambda. This allows for complex operations while maintaining thread-safety..
		// as the lock will be held.
		template<typename Func>
		void Modify(Func&& func) {
			std::lock_guard<std::mutex> lock(mutex);
			func(value);
		}

	private:
		T value;
		std::mutex mutex;

	};

	template<typename T>
	class ThreadSafePrimitive {
	public:
		// ASSERT doesn't work for type traits, so static_assert is used here
		static_assert(std::is_fundamental_v<T>, "ThreadSafePrimitive only supports primitive types (int, bool, float, etc.)");
		ThreadSafePrimitive(T value = T{}) : atomicValue(value) {}

		// std::memory_order_seq_cst (sequential consistency) is the safest level of memory ordering..
		// but also the slowest. Weaker orderings can introduce many bugs when not used carefully.

		T Get(std::memory_order order = std::memory_order_seq_cst) const {
			return this->atomicValue.load(order);
		}

		void Set(T new_value, std::memory_order order = std::memory_order_seq_cst) {
			this->atomicValue.store(new_value, order);
		}

		// Type alias prevents us from having explicitly define the type again
		template<typename U = T>
		auto Add(U increment, std::memory_order order = std::memory_order_seq_cst) -> std::enable_if_t<std::is_arithmetic_v<U>, T>
		{
			return this->atomicValue.fetch_add(increment, order);
		}

		bool CompareAndSwap(T expected, T desired,
			std::memory_order order = std::memory_order_seq_cst) {
			return this->atomicValue.compare_exchange_strong(expected, desired, order);
		}

		// TODO: More operator overloads so this could be used more naturally

		ThreadSafePrimitive& operator=(T newValue) {
			Set(newValue);
			return *this;
		}

	private:
		std::atomic<T> atomicValue;
	};

}
