#pragma once
#include "KindInfoInterface.h"
#include "TypeInfo.h"

namespace Reflection {

	struct MemberInfo
	{
		MemberInfo(std::string& name, TypeInfo& type) : name(name), type(type) {}

		std::string name;
		TypeInfo type;
	};

	struct FunctionSignature
	{
		FunctionSignature(TypeInfo& returnType, std::vector<TypeInfo> parameterTypes) : returnType(returnType), parameterTypes(parameterTypes) {}

		TypeInfo returnType;
		std::vector<TypeInfo> parameterTypes;
	};

	struct FunctionInfo
	{
		FunctionInfo(std::string& name, FunctionSignature signature) : name(name), signature(signature) {}

		std::string name;
		FunctionSignature signature;
	};

	struct EnumInfo
	{
		EnumInfo(std::string& name, int position) : name(name), position(position) {}

		std::string name;
		int position;
	};

    template<typename T>
    KindInfo KindInfo::Create(std::string name)
	{
		KindInfo k = KindInfo();
		k.name = name;
		k.size = sizeof(T);
		k.alignment = alignof(T);

		if constexpr (std::is_integral_v<T>)
		{
			k.categories = k.categories | KindCategories::Integral;
		}
		if constexpr (std::is_floating_point_v<T>)
		{
			k.categories = k.categories | KindCategories::FloatingPoint;
		}
		if constexpr (std::is_enum_v<T>)
		{
			k.categories = k.categories | KindCategories::Enum;
		}
		if constexpr (std::is_class_v<T>)
		{
			k.categories = k.categories | KindCategories::Class;
		}
		if constexpr (std::is_function_v<T>)
		{
			k.categories = k.categories | KindCategories::Function;
		}

		return k;
	}

	template<>
	inline KindInfo KindInfo::Create<void>(std::string name)
	{
		KindInfo k = KindInfo();
		k.name = name;
		return k;
	}

    template<typename T, typename C>
    void KindInfo::AddClassMember(std::string name, T C::* member)
    {
		ASSERT((this->categories & KindCategories::Class) >> 3, "Cannot register a class member to a non-class type.")

		if (this->classMembers == nullptr)
		{
			this->classMembers = std::make_shared<std::vector<MemberInfo>>();
		}
		this->classMembers->push_back(MemberInfo(name, TypeInfo::Get<T>()));
    }

	template<typename T>
	void KindInfo::AddFunctionSignature(std::string name)
	{
		ASSERT((this->categories & KindCategories::Function) >> 4, "Cannot register a function signature to a non-function type.")
		// TODO:
		// Split T into return type and parameter tuple type
		// run TypeInfo::Get<T>() on every type.
		// move result into function signature.

		//this->functionInfo.reset(FunctionInfo(name, FunctionSignature())); 
	}

	inline void KindInfo::AddEnumMember(std::string name, int position)
	{
		ASSERT((this->categories & KindCategories::Enum) >> 2, "Cannot register an enum member to a non-enum type.")
		if (this->enumMembers == nullptr)
		{
			this->enumMembers = std::make_shared<std::vector<EnumInfo>>();
		}
		this->enumMembers->push_back(EnumInfo(name, position));
	}
}