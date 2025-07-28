#pragma once
#include "KindRegistry.h"
#include "KindInfo.h"

#define REFLECT_PRIMITIVE(name, type) static ::Reflection::KindRegistrar registerKind_##name = ::Reflection::KindRegistrar(typeid(type), std::make_unique<::Reflection::KindInfo>(::Reflection::KindInfo::Create<##type>(#type)));

#define REFLECT_CLASS(name) ::Reflection::KindRegistrar registerKind_##name = ::Reflection::KindRegistrar(typeid(name), std::make_unique<::Reflection::KindInfo>( \
	[]() -> ::Reflection::KindInfo \
{ \
	::Reflection::KindInfo k = ::Reflection::KindInfo::Create<name>(#name);

#define REFLECT_MEMBER(className, name) k.AddClassMember(#name, &##className::##name);

#define END_CLASS() return k; \
}() \
)); 

