#pragma once
#include "KindInfoInterface.h"
#include "TypeInfo.h"
#include <algorithm>

namespace Reflection {

	struct MemberInfo
	{
		MemberInfo(std::string& name, TypeInfo& type, std::function<void* (void*)> getter, std::function<void(void*, void*)> setter) 
			: name(name), type(type), getter(getter), setter(setter)
		{}

		const std::string name;
		const TypeInfo type;
		const std::function<void* (void*)> getter;
		const std::function<void(void*, void*)> setter;
	};

	struct FunctionSignature
	{
		FunctionSignature(TypeInfo& returnType, std::vector<TypeInfo> parameterTypes) : returnType(returnType), parameterTypes(parameterTypes) {}

		TypeInfo returnType;
		std::vector<TypeInfo> parameterTypes;
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
		
		this->classMembers->push_back(
			MemberInfo(name, TypeInfo::Get<T>(),
			[member](void* instance) -> void* {
				return &(static_cast<C*>(instance)->*member);
			},
			[member](void* instance, const void* value) {
				(static_cast<C*>(instance)->*member) = *static_cast<const T*>(value);
			})
		);

		return *this;
    }

	template<class Class, typename R, typename ...Args>
	R KindInfo::CallMethod(std::string name, Class* ptr, Args ...args) const
	{
		const MemberInfo& methodInfo = this->GetMethodInfo(name);

		//TBD
		return R();
	}

	template<typename T, class Class>
	void KindInfo::SetProperty(std::string name, Class* ptr, T value) const
	{
		const MemberInfo& propInfo = this->GetPropertyInfo(name);
		propInfo.setter(ptr, &value);
	}

	template<typename T, class Class>
	T KindInfo::GetProperty(std::string name, Class* ptr) const
	{
		const MemberInfo& propInfo = this->GetPropertyInfo(name);
		void* result = propInfo.getter(ptr);
		return *static_cast<T*>(result);
	}

	KindInfo& KindInfo::AddEnumMember(std::string name, int position)
	{
		ASSERT((this->categories & KindCategories::Enum) >> 2, "Cannot register an enum member to a non-enum type.")
		if (this->enumMembers == nullptr)
		{
			this->enumMembers = std::make_shared<std::vector<EnumInfo>>();
		}
		this->enumMembers->push_back(EnumInfo(name, position));
		return *this;
	}

	const MemberInfo& KindInfo::GetMemberInfo(std::string name) const
	{
		std::vector<MemberInfo>::iterator result = std::find_if(this->classMembers->begin(), this->classMembers->end(), 
		[name](const MemberInfo& value){
			return value.name == name;
		});

		ASSERT(result != this->classMembers->end(), ("Class '" + this->name + "' does not have reflected member '" + name + "'").c_str())
		return *result;
	}

	const MemberInfo& KindInfo::GetMethodInfo(std::string name) const
	{
		std::vector<MemberInfo>::iterator result = std::find_if(this->classMembers->begin(), this->classMembers->end(),
		[name](const MemberInfo& value)
		{
			return value.name == name && value.type.kind->categories & KindCategories::Function >> 0;
		});

		ASSERT(result != this->classMembers->end(), ("Class '" + this->name + "' does not have reflected method '" + name + "'").c_str())
		return *result;
	}

	const MemberInfo& KindInfo::GetPropertyInfo(std::string name) const
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