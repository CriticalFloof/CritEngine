#pragma once
#include <string>
#include <iostream>
#include <filesystem>

#define CE_EXPAND_MACRO(x) x
#define CE_STRINGIFY_MACRO(x) #x

#ifdef CE_PLATFORM_WINDOWS
    #ifdef ENGINE_BUILD_DLL
         #define ENGINE_API __declspec(dllexport)
    #else
         #define ENGINE_API __declspec(dllimport)
    #endif
#else
    #error CritEngine can only support the following platforms: "Windows"
#endif

#ifdef CE_DEBUG
    #define CE_ENABLE_ASSERTS

    #if defined(CE_PLATFORM_WINDOWS)
        #define CE_DEBUGBREAK() __debugbreak()
    #elif defined(CE_PLATFORM_LINUX)
        #include <signal.h>
        #define CE_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
#else
    #define CE_DEBUGBREAK()
#endif

#ifdef CE_ENABLE_ASSERTS
    #define CE_INTERNAL_ASSERT_IMPL(check, msg, ...) { if(!(check)) { printf(msg, __VA_ARGS__); CE_DEBUGBREAK(); } }
    #define CE_INTERNAL_ASSERT_WITH_MSG(check, ...) CE_INTERNAL_ASSERT_IMPL(check, "Assertion failed: %s", __VA_ARGS__)
    #define CE_INTERNAL_ASSERT_NO_MSG(check) CE_INTERNAL_ASSERT_IMPL(check, "Assertion '%s' failed at %s:%s", CE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string().c_str(), std::to_string(__LINE__).c_str())

    #define CE_INTERNAL_ASSERT_PICK_MACRO(arg1, arg2, macro, ...) macro
    #define CE_INTERNAL_ASSERT_GET_MACRO(...) CE_EXPAND_MACRO( CE_INTERNAL_ASSERT_PICK_MACRO(__VA_ARGS__, CE_INTERNAL_ASSERT_WITH_MSG, CE_INTERNAL_ASSERT_NO_MSG) )

    // Assert has to be called with a boolean condition and optionally with a string message, you can check CE_INTERNAL_ASSERT_IMPL to verify.
    #define ASSERT(...) CE_EXPAND_MACRO( CE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
    #define ASSERT(...)
#endif
