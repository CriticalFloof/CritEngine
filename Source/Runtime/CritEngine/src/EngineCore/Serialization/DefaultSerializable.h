#pragma once
#include <string>

#include "Serializable.h"
#include "../Core/Reflection/Reflectable.h"
#include "../Core/Reflection/Base.h"

namespace Engine {

	/*
	Implements generic serialization through the reflection of exposed data members.
	*/
	class DefaultSerializable : public ISerializable
	{
	public:

		virtual void Serialize(std::ostream& write) override { return; }

		template<typename Class>
		std::enable_if_t<std::is_base_of_v<Reflectable, Class>> Serialize(std::ostream& write)
		{	
			Class* obj = static_cast<Class*>(this);
			TypeInfo<Class> info = obj->GetTypeInfo();
			
			write << "Class: " << info.name << "\n";

			write << "Properties[\n";
			for (TypeInfo<Class>::Property& prop : info.properties)
			{
				write << "    ";
				write << prop.name + ": ";
				write << std::string(prop.type.name()) + " | ";
				write << PropertyToString<Class>(obj, prop) + "\n";
			}
			write << "]\n";
		}

		virtual void Deserialize(std::istream& source) override
		{

		}

	private:
		
	};

	template<typename Class>
	static std::string PropertyToString(Class* instance, typename TypeInfo<Class>::Property& prop)
	{

		if (prop.type == typeid(int))
		{
			return std::to_string(*prop.Get<int>(instance));
		}
		else if (prop.type == typeid(float))
		{
			return std::to_string(*prop.Get<float>(instance));
		}
		else if (prop.type == typeid(std::string))
		{
			return *prop.Get<std::string>(instance);
		}
		else
		{
			ASSERT(false, "Conversion from " + prop.name + " to string isn't supported!")
		}
	}

	template<typename Class>
	static void StringToProperty(Class* instance, typename TypeInfo<Class>::Property& prop, std::string value)
	{
		if (prop.type == typeid(int))
		{
			prop.Set(instance, std::stoi(value));
		}
		else if (prop.type == typeid(float))
		{
			prop.Set(instance, std::stof(value));
		}
		else if (prop.type == typeid(std::string))
		{
			prop.Set(instance, value);
		}
		else
		{
			ASSERT(false, "Conversion from string to "+ prop.name +" isn't supported!")
		}
	}

}