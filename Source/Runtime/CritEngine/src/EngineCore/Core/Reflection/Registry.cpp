#include "Registry.h"

namespace Reflection {

	class TypeRegistryHandle::TypeRegistry
	{
	public:
		static TypeRegistry& Instance()
		{
			static TypeRegistry instance;
			return instance;
		}

		void SetMetadata(std::type_index type, std::unique_ptr<TypeInfo> metadata) {
			std::cout << "Registered Type " + metadata->name << std::endl;
			// We assert here because it is very bad practice to modify the reflection data after engine initalization.
			ASSERT(this->Instance().registeredTypes.find(type) == this->Instance().registeredTypes.end(), "Overwriting reflected type information is not allowed.")
				this->Instance().registeredTypes[type] = *metadata.get();
			std::cout << this->Instance().registeredTypes.size() << std::endl;
		};

		const TypeInfo* GetMetadata(std::type_index type)
		{
			return &this->Instance().registeredTypes.at(type);
		};

	private:
		std::unordered_map<std::type_index, TypeInfo> registeredTypes;
	};

	void TypeRegistryHandle::SetMetadata(std::type_index type, std::unique_ptr<TypeInfo> metadata)
	{
		impl->SetMetadata(type, std::move(metadata));
	}

	const TypeInfo* TypeRegistryHandle::GetMetadata(std::type_index type)
	{
		return impl->GetMetadata(type);
	}

	TypeRegistryHandle TypeRegistrar::registryHandle = TypeRegistryHandle();
}