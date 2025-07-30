#pragma once 
#include <string>
#include <cstdio>
#include <type_traits>
#include <memory>
#include <vector>
#include <functional>

namespace Reflection {

	class TypeInfo;
	struct MemberInfo;
	struct FunctionInfo;
	struct FunctionSignature;
	struct EnumInfo;

	enum KindCategories : uint8_t
	{
		Function = 1 << 0,
		Class = 1 << 1,
		Enum = 1 << 2,
	};
	inline KindCategories operator|(KindCategories lhs, KindCategories rhs)
	{
		return static_cast<KindCategories>(
			static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)
		);
	}
	inline KindCategories operator&(KindCategories lhs, KindCategories rhs)
	{
		return static_cast<KindCategories>(
			static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)
		);
	}

	class KindInfo
	{
	public:
		template<typename T>
		static KindInfo Create(std::string name);
		template<>
		static KindInfo Create<void>(std::string name);

		// Setters

		template<typename R, typename ...Args>
		KindInfo& SetFunctionSignature(std::function<R(Args...)> func); 
		template<typename T, typename C>
		KindInfo& AddClassMember(std::string name, T C::* member);
		KindInfo& AddEnumMember(std::string name, int position);

		// Class introspection
		
		// Searches for a class member that matches a given name
		const MemberInfo& GetMemberInfo(std::string name) const;
		// Like GetClassMember, but also only matches for function types.
		const MemberInfo& GetMethodInfo(std::string name) const;
		// Like GetClassMember, but also only matches for non-function types.
		const MemberInfo& GetPropertyInfo(std::string name) const;

		// Class code injection

		template<class Class, typename R, typename ...Args>
		R CallMethod(std::string name, Class* ptr, Args... args) const;

		template<typename T, class Class>
		void SetProperty(std::string name, Class* ptr, T value) const;

		template<typename T, class Class>
		T GetProperty(std::string name, Class* ptr) const;

		// Enum introspection

		const EnumInfo& GetEnumMember(size_t position) const;

		std::string name;
		std::size_t size = 0;
		std::size_t alignment = 0;
		KindCategories categories = static_cast<KindCategories>(0);

		std::shared_ptr<std::vector<MemberInfo>> classMembers;
		std::shared_ptr<std::vector<EnumInfo>> enumMembers;
		std::shared_ptr<FunctionInfo> functionInfo;

	private:
		
	};

	// Primitive Group Functions
	inline bool IsIntegral(const KindInfo& info)
	{
		return 
			(info.categories & (KindCategories::Class | KindCategories::Function | KindCategories::Enum)) == 0 && 
			info.name == "short" ||
			info.name == "int" ||
			info.name == "long" ||
			info.name == "long_long" ||
			info.name == "unsigned_short" ||
			info.name == "unsigned_int" ||
			info.name == "unsigned_long" ||
			info.name == "unsigned_long_long" ||
			info.name == "bool" ||
			info.name == "char" ||
			info.name == "signed_char" ||
			info.name == "unsigned_char" ||
			info.name == "wchar_t" ||
			info.name == "char16_t" ||
			info.name == "char32_t";
	}

	inline bool IsFloatingPoint(const KindInfo& info)
	{
		return
			(info.categories & (KindCategories::Class | KindCategories::Function | KindCategories::Enum)) == 0 &&
			info.name == "float" ||
			info.name == "double" ||
			info.name == "long_double";
	}

	inline bool IsSigned(const KindInfo& info)
	{
		return
			(info.categories & (KindCategories::Class | KindCategories::Function | KindCategories::Enum)) == 0 &&
			info.name == "short" ||
			info.name == "int" ||
			info.name == "long" ||
			info.name == "long_long" ||
			info.name == "signed_char";
	}

	inline bool IsUnsigned(const KindInfo& info)
	{
		return
			(info.categories & (KindCategories::Class | KindCategories::Function | KindCategories::Enum)) == 0 &&
			info.name == "unsigned_short" ||
			info.name == "unsigned_int" ||
			info.name == "unsigned_long" ||
			info.name == "unsigned_long_long" ||
			info.name == "unsigned_char";
	}
}