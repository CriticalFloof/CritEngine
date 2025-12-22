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
class ENGINE_API Debug {
public:
	template<typename... Args>
	static void Log(Args&&... params) {
		std::string str = Process(std::forward<Args>(params)...);
		printf("\033[%s;1;1m[%s] %s \033[0m\n", std::to_string(Color::WHITE).c_str(), GetCurrentTimestamp().c_str(), str.c_str());
	}

	template<typename... Args>
	static void LogWarning(Args&&... params) {
		std::string str = Process(std::forward<Args>(params)...);
		printf("\033[%s;1;1m[%s] %s \033[0m\n", std::to_string(Color::YELLOW).c_str(), GetCurrentTimestamp().c_str(), str.c_str());
	}

	template<typename... Args>
	static void LogError(Args&&... params) {
		std::string str = Process(std::forward<Args>(params)...);
		printf("\033[%s;1;1m[%s] %s \033[0m\n", std::to_string(Color::RED).c_str(), GetCurrentTimestamp().c_str(), str.c_str());
	}

private:
	enum Color { RED = 31, YELLOW = 33, WHITE = 37 };

	// This function converts arguments into strings using std::to_string, so all supported types
	// will automatically be converted without having to break their components down and/or use std::to_string
	template<typename T>
	static std::string Process(T&& arg) {

		// Using SFINAE (Substition failure is not an error) to reduce code for handling types
		// (Concepts is safer and preferred but only availble in later C++ standards)
		if constexpr (std::is_arithmetic_v<std::decay_t<T>> && !std::is_same_v<std::decay_t<T>, bool>) {
			return std::to_string(arg);
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
			return arg ? "True" : "False";
		}
		else if constexpr (std::is_convertible_v<std::decay_t<T>, std::string>) {
			return std::string(arg);
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, Engine::Vector2>) {
			return "(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + ")";
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, Engine::Vector3>) {
			return "(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + "," + std::to_string(arg.z) + ")";
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, Engine::Vector4>) {
			return "(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + "," + std::to_string(arg.z) + "," + std::to_string(arg.w) + ")";
		}
		else {
			ASSERT(always_false<T>::value, "Unsupported type passed to Logger");
		}
	}

	// Recursive processing of every argument
	template<typename T, typename... Args>
	static std::string Process(T&& first, Args&&... rest) {
		return Process(std::forward<T>(first)) + " " + Process(std::forward<Args>(rest)...);
	}

	template<typename T>
	struct always_false : std::false_type {};

	const static std::string GetCurrentTimestamp();
};

const void ENGINE_API LogInfo(const std::string& label, const std::string& message);
const void ENGINE_API LogWarning(const std::string& label, const std::string& message);
const void ENGINE_API LogError(const std::string& label, const std::string& message);