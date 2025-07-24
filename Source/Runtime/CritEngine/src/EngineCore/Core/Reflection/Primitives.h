#pragma once
#include "Registry.h"


namespace Reflection {
    #ifdef ENGINE_BUILD_DLL
	namespace {
		TypeRegistrar registerType_int = TypeRegistrar(typeid(int), std::make_unique<TypeInfo>(TypeInfo::Create<int>("int")));
	}
    #endif
}