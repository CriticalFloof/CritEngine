#pragma once
#include <mutex>
#include <type_traits>
#include <optional>

#include "../Core/Base.h"

ENGINE_API unsigned int getThreadCountEstimate();

namespace Engine
{
    // Recommended for use with more complex structures
    template <typename T>
    class ThreadSafeVariable
    {
    public:
        ThreadSafeVariable(T value) : m_value(value)
        {
        }

        T get()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_value;
        }

        void get(T new_value)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_value = new_value;
        }

        // Attempts a non-blocking read
        std::optional<T> tryGet() const
        {
            if (!m_mutex.try_lock())
            {
                return std::nullopt;
            }
            std::lock_guard<std::mutex> lock(m_mutex, std::adopt_lock);
            return m_value;
        }

        // Modify the value atomically using a lambda. This allows for complex operations while maintaining thread-safety..
        // as the lock will be held.
        template <typename Func>
        void modify(Func&& func)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            func(m_value);
        }

    private:
        T m_value;
        std::mutex m_mutex;
    };

    template <typename T>
    class ThreadSafePrimitive
    {
    public:
        // ASSERT doesn't work for type traits, so static_assert is used here
        static_assert(std::is_fundamental_v<T>,
                      "ThreadSafePrimitive only supports primitive types (int, bool, float, etc.)");

        ThreadSafePrimitive(T value = T{}) : m_atomicValue(value)
        {
        }

        // std::memory_order_seq_cst (sequential consistency) is the safest level of memory ordering..
        // but also the slowest. Weaker orderings can introduce many bugs when not used carefully.

        T get(std::memory_order order = std::memory_order_seq_cst) const
        {
            return m_atomicValue.load(order);
        }

        void set(T new_value, std::memory_order order = std::memory_order_seq_cst)
        {
            m_atomicValue.store(new_value, order);
        }

        // Type alias prevents us from having explicitly define the type again
        template <typename U = T>
        std::enable_if_t<std::is_arithmetic_v<U>, T> add(U increment,
                                                         std::memory_order order = std::memory_order_seq_cst)
        {
            return m_atomicValue.fetch_add(increment, order);
        }

        bool compareAndSwap(T expected, T desired,
                            std::memory_order order = std::memory_order_seq_cst)
        {
            return m_atomicValue.compare_exchange_strong(expected, desired, order);
        }

        // TODO: More operator overloads so this could be used more naturally

        ThreadSafePrimitive& operator=(T new_value)
        {
            set(new_value);
            return *this;
        }

    private:
        std::atomic<T> m_atomicValue;
    };
}
