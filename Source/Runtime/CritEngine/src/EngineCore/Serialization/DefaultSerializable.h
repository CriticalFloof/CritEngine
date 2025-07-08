#pragma once
#include <string>

#include "Serializable.h"
#include "../Core/Reflection/Reflectable.h"
#include "../Core/Reflection/Base.h"

namespace Engine {

	class DefaultSerializable : public ISerializable
	{
	public:

		virtual std::string Serialize() override { return std::string(""); }

		template<typename Class>
		std::enable_if_t<std::is_base_of_v<Reflectable, Class>, std::string> Serialize()
		{	
			Class* obj = static_cast<Class*>(this);
			TypeInfo<Class> info = obj->GetTypeInfo();
			std::string result = "";

			result += "Class: ";
			result += info.name + "\n";

			result += "Properties[\n";
			for (TypeInfo<Class>::Property& prop : info.properties)
			{
				result += "    ";
				result += prop.name + ": ";
				result += std::string(prop.type.name()) + " | ";
				result += PropertyToString<Class>(obj, prop) + "\n";
			
			}
			result += "]\n";


			std::cout << "Hello Serialization! " << result << "\n";
			return result;
		}

		virtual void Deserialize(std::string source) override
		{

		}

	private:
		
	};

	template<typename Class>
	static std::string PropertyToString(Class* instance, typename TypeInfo<Class>::Property& property)
	{
		if (property.type == typeid(int))
		{
			return std::to_string(*property.Get<int>(instance));
		}
		else if (property.type == typeid(float))
		{
			return std::to_string(*property.Get<float>(instance));
		}
		else
		{
			ASSERT(false, "Conversion from property to string isn't supported!")
		}

		return std::string();
	}

	template<typename Class>
	static void StringToProperty(Class* instance, typename TypeInfo<Class>::Property& property, std::string value)
	{
		if (property.type == typeid(int))
		{
			property.Set<int>(instance, std::stoi(value));
		}
		else if (property.type == typeid(float))
		{
			property.Set<float>(instance, std::stof(value));
		}
		else
		{
			ASSERT(false, "Conversion from string to property isn't supported!")
		}

		return std::string();
	}

}