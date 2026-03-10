#pragma once
#include <functional>
#include <any>
#include <tuple>
#include <memory>

#include "../Logging/Logger.h"

namespace Engine
{
    // Some of these may or may not be used in the future
    /*
    enum class EventType {
        None = 0,
        WindowClose, WindowResize,
        WindowFocusLost, WindowFocus,
        WindowMoved,
        OnTick, OnRender,
        KeyPressed, KeyReleased, KeyJustPressed,
        MousePressed, MouseReleased, MouseJustPressed, MouseMoved, MouseScroll
    };
    */

    static int g_event_id_counter = 0;

    template <class T>
    int getId()
    {
        static int event_id = g_event_id_counter++;
        return event_id;
    };

    template <typename Tuple>
    struct tuple_to_function;

    template <typename... Args>
    struct tuple_to_function<std::tuple<Args...>>
    {
        using type = std::function<void(Args...)>;
    };

    template <typename... input_t>
    using tuple_cat_t =
    decltype(std::tuple_cat(
        std::declval<input_t>()...
    ));

    template <typename Sig, typename DerivedParent = void>
    struct Event
    {
        using Parent = DerivedParent;
        using Signature = tuple_cat_t<Sig, std::conditional_t<
                                          std::is_void_v<DerivedParent>, std::tuple<>, typename
                                          DerivedParent::Signature>>;
        using FunctionType = tuple_to_function<Signature>::type;
    };

    template <typename Sig>
    struct Event<Sig>
    {
        using Signature = Sig;
        using FunctionType = tuple_to_function<Sig>::type;
    };

    struct AnyEvent : Event<std::tuple<>>
    {
    };

    class EventEmitter
    {
    public:
        template <typename DerivedEvent>
        void addListener(DerivedEvent::FunctionType event_handler)
        {
            const uintptr_t event_id = getId<DerivedEvent>();

            std::function<void(std::vector<std::any>)> storedCallback = [this, event_handler
                ](const std::vector<std::any>& args)
            {
                this->unpack_and_call(event_handler, args);
            };
            this->m_listeners[event_id].push_back(storedCallback);
        };

        template <typename DerivedEvent, typename... Args>
        std::enable_if_t<
            std::is_same_v<std::tuple<std::decay_t<Args>...>, typename DerivedEvent::Signature>
        >
        emit(Args&&... args)
        {
            const uintptr_t event_id = getId<DerivedEvent>();
            const std::vector<std::any> packed_args = this->to_any_vector(args...);

            for (int i = 0; i < this->m_listeners[event_id].size(); i++)
            {
                this->m_listeners[event_id][i](packed_args);
            }
        }

    private:
        std::unordered_map<uintptr_t, std::vector<std::function<void(std::vector<std::any>)>>> m_listeners;

        template <typename T>
        T any_cast_from_vector(const std::vector<std::any>& vec, std::size_t index)
        {
            return std::any_cast<T>(vec[index]);
        }

        template <typename... Args, std::size_t... Is>
        void unpack_and_call_impl(std::function<void(Args...)> func, const std::vector<std::any>& vec,
                                  std::index_sequence<Is...>)
        {
            func(any_cast_from_vector<Args>(vec, Is)...);
        }

        template <typename... Args>
        void unpack_and_call(std::function<void(Args...)> func, const std::vector<std::any>& vec)
        {
            unpack_and_call_impl(func, vec, std::index_sequence_for<Args...>{});
        }

        template <typename... Args>
        std::vector<std::any> to_any_vector(Args&&... args)
        {
            return {std::any(std::forward<Args>(args))...};
        }
    };
}
