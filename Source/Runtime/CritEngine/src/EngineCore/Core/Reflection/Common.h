#pragma once
#include "KindRegistry.h"
#include "KindInfo.h"

// Shorthand for REFLECT_KIND which automatically resolves the kindinfo for a primitive; macro includes a name parameter to resolve types with spaces in them.
#define REFLECT_PRIMITIVE(name, type) static ::Reflection::KindRegistrar registerKind_##name = ::Reflection::KindRegistrar(typeid(type), std::make_unique<::Reflection::KindInfo>(::Reflection::KindInfo::Create<##type>(#type)));

#define REFLECT_KIND(type, kind) static ::Reflection::KindRegistrar registerKind_##type = ::Reflection::KindRegistrar(typeid(type), std::make_unique<::Reflection::KindInfo>(kind));

