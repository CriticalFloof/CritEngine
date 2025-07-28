#pragma once 
#include <string>
#include <cstdio>
#include <type_traits>
#include <memory>
#include <vector>

namespace Reflection {

	class TypeInfo;
	struct MemberInfo;
	struct FunctionInfo;
	struct FunctionSignature;
	struct EnumInfo;

	enum KindCategories : uint8_t
	{
		Integral = 1 << 0,
		FloatingPoint = 1 << 1,
		Enum = 1 << 2,
		Class = 1 << 3,
		Function = 1 << 4,
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

		template<typename T, typename C>
		void AddClassMember(std::string name, T C::* member);
		template<typename T>
		void AddFunctionSignature(std::string name);
		void AddEnumMember(std::string name, int position);

		std::string name;
		std::size_t size = 0;
		std::size_t alignment = 0;
		KindCategories categories = static_cast<KindCategories>(0);

		std::shared_ptr<std::vector<MemberInfo>> classMembers;
		std::shared_ptr<std::vector<EnumInfo>> enumMembers;
		std::shared_ptr<FunctionInfo> functionInfo;

	private:
		//
	};
}