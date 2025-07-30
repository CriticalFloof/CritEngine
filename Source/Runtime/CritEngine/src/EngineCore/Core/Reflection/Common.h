#pragma once
#include "KindRegistry.h"
#include "KindInfo.h"

// Shorthand for REFLECT_KIND which automatically resolves the kindinfo for a primitive; macro includes a name parameter to resolve types with spaces in them.
#define REFLECT_PRIMITIVE(name, type) static ::Reflection::KindRegistrar registerKind_##name = ::Reflection::KindRegistrar(typeid(type), std::make_unique<::Reflection::KindInfo>(::Reflection::KindInfo::Create<##type>(#type)));

#define REFLECT_KIND(type, kind) static ::Reflection::KindRegistrar registerKind_##type = ::Reflection::KindRegistrar(typeid(type), std::make_unique<::Reflection::KindInfo>(kind));

// Primitive Registration
#ifdef ENGINE_BUILD_DLL
namespace {
    REFLECT_PRIMITIVE(short, short)
    REFLECT_PRIMITIVE(int, int)
    REFLECT_PRIMITIVE(long, long)
    REFLECT_PRIMITIVE(long_long, long long)

    REFLECT_PRIMITIVE(unsigned_short, unsigned short)
    REFLECT_PRIMITIVE(unsigned_int, unsigned int)
    REFLECT_PRIMITIVE(unsigned_long, unsigned long)
    REFLECT_PRIMITIVE(unsigned_long_long, unsigned long long)

    REFLECT_PRIMITIVE(char, char)
    REFLECT_PRIMITIVE(signed_char, signed char)
    REFLECT_PRIMITIVE(unsigned_char, unsigned char)
    REFLECT_PRIMITIVE(wchar_t, wchar_t)
    REFLECT_PRIMITIVE(char16_t, char16_t)
    REFLECT_PRIMITIVE(char32_t, char32_t)

    REFLECT_PRIMITIVE(float, float)
    REFLECT_PRIMITIVE(double, double)
    REFLECT_PRIMITIVE(long_double, long double)

    REFLECT_PRIMITIVE(bool, bool)

    REFLECT_PRIMITIVE(void, void)
}
#endif