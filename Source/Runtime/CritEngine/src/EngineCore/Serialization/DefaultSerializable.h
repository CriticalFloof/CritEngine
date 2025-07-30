#pragma once
#include <string>

#include "Serializable.h"
#include "../Core/Reflection/Common.h"

namespace Engine {

	static std::string PropertyToString(void* instance, const ::Reflection::MemberInfo& prop)
	{
		if (::Reflection::IsIntegral(*prop.type.kind))
		{
			void* value = prop.getter(instance);
			uint8_t buffer[sizeof(uint64_t)] = {0};
			std::memcpy(buffer, value, prop.type.kind->size);

			if (::Reflection::IsSigned(*prop.type.kind))
			{
				return std::to_string(*static_cast<int64_t*>(static_cast<void*>(buffer)));
			}
			else
			{
				return std::to_string(*static_cast<uint64_t*>(static_cast<void*>(buffer)));
			}
		}
		else if (::Reflection::IsFloatingPoint(*prop.type.kind))
		{
			void* value = prop.getter(instance);
			if (prop.type.kind->name == "float")
			{
				return std::to_string(*static_cast<float*>(value));
			}
			else if (prop.type.kind->name == "double")
			{
				return std::to_string(*static_cast<double*>(value));
			}
			else
			{
				return std::to_string(*static_cast<long double*>(value));
			}
		}
		else
		{
			ASSERT(false, ("Conversion from " + prop.type.kind->name + " to string isn't supported!").c_str())
		}
	}

	static void StringToProperty(void* instance, const ::Reflection::MemberInfo& prop, std::string value)
	{
		if (::Reflection::IsIntegral(*prop.type.kind))
		{
			if (::Reflection::IsSigned(*prop.type.kind))
			{
				int64_t deserializedValue = std::stoll(value);
				prop.setter(instance, &deserializedValue);
			}
			else
			{
				uint64_t deserializedValue = std::stoull(value);
				prop.setter(instance, &deserializedValue);
			}
		}
		else if (::Reflection::IsFloatingPoint(*prop.type.kind))
		{
			if (prop.type.kind->name == "float")
			{
				float deserializedValue = std::stof(value);
				prop.setter(instance, &deserializedValue);
			}
			else if (prop.type.kind->name == "double")
			{
				double deserializedValue = std::stod(value);
				prop.setter(instance, &deserializedValue);
			}
			else
			{
				long double deserializedValue = std::stold(value);
				prop.setter(instance, &deserializedValue);
			}
		}
		else
		{
			ASSERT(false, ("Conversion from string to " + prop.type.kind->name + " isn't supported!").c_str())
		}
	}

	/*
	Implements generic serialization through the reflection of exposed data members.
	*/
	class DefaultSerializable : public ISerializable
	{
	public:

		virtual bool Serialize(std::ostream& dest) override
		{
			return false; // We can't serialize the object without passing in the derived class type
		}
		virtual bool Deserialize(std::istream& source) override
		{
			return false;
		}

		template<typename C>
		bool Serialize(std::ostream& dest)
		{
			::Reflection::TypeInfo info = ::Reflection::TypeInfo::Get<C>();

			dest << "Class: " << info.kind->name << "\n";

			dest << "Properties[\n";
			for (::Reflection::MemberInfo& member : *info.kind->classMembers)
			{
				// Functions can't be serialized.
				if (member.type.kind->categories & ::Reflection::KindCategories::Function >> 0) continue;

				dest << "    ";
				dest << member.name + ": ";
				dest << std::string(member.type.kind->name) + " | ";
				dest << PropertyToString(static_cast<C*>(this), member) + "\n";
			}
			dest << "]\n";

			return true;
		}

		template<typename C>
		bool Deserialize(std::istream& source)
		{
			return false;
		}



	private:
		
	};
}