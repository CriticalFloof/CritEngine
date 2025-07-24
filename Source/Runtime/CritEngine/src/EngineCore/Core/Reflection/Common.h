#pragma once
#include "Registry.h"

#define REFLECT_PRIMITIVE(name, type) TypeRegistrar registerType_##name = TypeRegistrar(typeid(type), std::make_unique<TypeInfo>(TypeInfo::Create<##type>(#type)));