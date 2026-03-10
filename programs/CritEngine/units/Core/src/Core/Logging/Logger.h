#pragma once

#include <iostream>
#include <cstdio>
#include <iomanip>
#include <string>
#include <chrono>
#include <sstream>
#include <utility>
#include <time.h>

#include "../Core/Base.h"

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"

// Improved logger class with automatic type deduction
class ENGINE_API Debug
{
public:
    template <typename... Args>
    static void log(Args&&... params)
    {
        std::string str = process(std::forward<Args>(params)...);
        printf("\033[%s;1;1m[%s] %s \033[0m\n", std::to_string(static_cast<int>(Color::White)).c_str(), getCurrentTimestamp().c_str(),
               str.c_str());
    }

    template <typename... Args>
    static void logWarning(Args&&... params)
    {
        std::string str = process(std::forward<Args>(params)...);
        printf("\033[%s;1;1m[%s] %s \033[0m\n", std::to_string(static_cast<int>(Color::Yellow)).c_str(), getCurrentTimestamp().c_str(),
               str.c_str());
    }

    template <typename... Args>
    static void logError(Args&&... params)
    {
        std::string str = process(std::forward<Args>(params)...);
        printf("\033[%s;1;1m[%s] %s \033[0m\n", std::to_string(static_cast<int>(Color::Red)).c_str(), getCurrentTimestamp().c_str(),
               str.c_str());
    }

private:
    enum class Color { Red = 31, Yellow = 33, White = 37 };

    // This function converts arguments into strings using std::to_string, so all supported types
    // will automatically be converted without having to break their components down and/or use std::to_string
    template <typename T>
    static std::string process(T&& arg)
    {
        // Using SFINAE (Substition failure is not an error) to reduce code for handling types
        // (Concepts is safer and preferred but only availble in later C++ standards)
        if constexpr (std::is_arithmetic_v<std::decay_t<T>> && !std::is_same_v<std::decay_t<T>, bool>)
        {
            return std::to_string(arg);
        }
        else if constexpr (std::is_same_v<std::decay_t<T>, bool>)
        {
            return arg ? "True" : "False";
        }
        else if constexpr (std::is_convertible_v<std::decay_t<T>, std::string>)
        {
            return std::string(arg);
        }
        else if constexpr (std::is_same_v<std::decay_t<T>, Engine::Vector2>)
        {
            return "(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + ")";
        }
        else if constexpr (std::is_same_v<std::decay_t<T>, Engine::Vector3>)
        {
            return "(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + "," + std::to_string(arg.z) + ")";
        }
        else if constexpr (std::is_same_v<std::decay_t<T>, Engine::Vector4>)
        {
            return "(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + "," + std::to_string(arg.z) + "," +
                std::to_string(arg.w) + ")";
        }
        else
        {
            ASSERT(always_false<T>::value, "Unsupported type passed to Logger");
        }
    }

    // Recursive processing of every argument
    template <typename T, typename... Args>
    static std::string process(T&& first, Args&&... rest)
    {
        return process(std::forward<T>(first)) + " " + process(std::forward<Args>(rest)...);
    }

    template <typename T>
    struct always_false : std::false_type
    {
    };

    const static std::string getCurrentTimestamp();
};

void ENGINE_API logInfo(const std::string& label, const std::string& message);
void ENGINE_API logWarning(const std::string& label, const std::string& message);
void ENGINE_API logError(const std::string& label, const std::string& message);
