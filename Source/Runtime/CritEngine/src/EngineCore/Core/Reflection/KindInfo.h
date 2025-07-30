#pragma once
#include "KindInfoInterface.h"
#include "TypeInfo.h"
#include <algorithm>

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
		EnumInfo(std::string& name, size_t position) : name(name), position(position) {}

		std::string name;
		size_t position;
	};

    template<typename T>
    KindInfo KindInfo::Create(std::string name)
	{
		KindInfo k = KindInfo();
		k.name = name;
		k.size = sizeof(T);
		k.alignment = alignof(T);

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

	template<typename R, typename ...Args>
	KindInfo& KindInfo::SetFunctionSignature(std::function<R(Args...)> func)
	{
		ASSERT((this->categories & KindCategories::Function) >> 0, "Cannot register a function signature to a non-function type.")
			// TODO:
			// Split T into return type and parameter tuple type
			// run TypeInfo::Get<T>() on every type.
			// move result into function signature.

			//this->functionInfo.reset(FunctionInfo(name, FunctionSignature())); 
			return *this;
	}

	template<typename T, typename C>
	KindInfo& KindInfo::AddClassMember(std::string name, T C::* member)
    {
		ASSERT((this->categories & KindCategories::Class) >> 1, "Cannot register a class member to a non-class type.")

		if (this->classMembers == nullptr)
		{
			this->classMembers = std::make_shared<std::vector<MemberInfo>>();
		}
		this->classMembers->push_back(MemberInfo(name, TypeInfo::Get<T>()));

		return *this;
    }

	inline KindInfo& KindInfo::AddEnumMember(std::string name, int position)
	{
		ASSERT((this->categories & KindCategories::Enum) >> 2, "Cannot register an enum member to a non-enum type.")
		if (this->enumMembers == nullptr)
		{
			this->enumMembers = std::make_shared<std::vector<EnumInfo>>();
		}
		this->enumMembers->push_back(EnumInfo(name, position));
		return *this;
	}

	const MemberInfo& KindInfo::GetClassMember(std::string name) const
	{
		std::vector<MemberInfo>::iterator result = std::find_if(this->classMembers->begin(), this->classMembers->end(), 
		[name](const MemberInfo& value){
			return value.name == name;
		});

		ASSERT(result != this->classMembers->end(), ("Class '" + this->name + "' does not have reflected member '" + name + "'").c_str())
		return *result;
	}

	inline const MemberInfo& KindInfo::GetClassMethod(std::string name) const
	{
		std::vector<MemberInfo>::iterator result = std::find_if(this->classMembers->begin(), this->classMembers->end(),
		[name](const MemberInfo& value)
		{
			return value.name == name && value.type.kind->categories & KindCategories::Function >> 0;
		});

		ASSERT(result != this->classMembers->end(), ("Class '" + this->name + "' does not have reflected method '" + name + "'").c_str())
		return *result;
	}

	inline const MemberInfo& KindInfo::GetClassProperty(std::string name) const
	{
		std::vector<MemberInfo>::iterator result = std::find_if(this->classMembers->begin(), this->classMembers->end(),
		[name](const MemberInfo& value)
		{
			return value.name == name && !(value.type.kind->categories & KindCategories::Function >> 0);
		});

		ASSERT(result != this->classMembers->end(), ("Class '" + this->name + "' does not have reflected property '" + name + "'").c_str())
		return *result;
	}

	const EnumInfo& KindInfo::GetEnumMember(size_t position) const
	{
		std::vector<EnumInfo>::iterator result = std::find_if(this->enumMembers->begin(), this->enumMembers->end(),
		[position](const EnumInfo& value)
		{
			return value.position == position;
		});

		ASSERT(result != this->enumMembers->end(), ("Enum '" + this->name + "' does not have reflected member '" + name + "'").c_str())
		return *result;
	}
}