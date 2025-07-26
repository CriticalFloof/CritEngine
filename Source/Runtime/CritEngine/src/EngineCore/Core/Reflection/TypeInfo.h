#pragma once
#include "KindInfo.h"
#include "SemanticStack.h"
#include "KindRegistry.h"
#include "Reduce.h"

namespace Reflection {

	class TypeInfo
	{
	public:

		template<typename T>
		static TypeInfo Get()
		{
			TypeInfo result = TypeInfo();
			KindRegistryHandle handle = KindRegistryHandle();

			result.semantics = SemanticStack::Create<T>();
			result.kind = handle.GetMetadata(typeid(reduce<T>));
		}

	private:
		KindInfo kind;
		SemanticStack semantics;
	};

}