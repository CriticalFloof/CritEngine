#include "KindRegistry.h"

namespace Reflection {

	class KindRegistryHandle::KindRegistry
	{
	public:
		static KindRegistry& Instance()
		{
			static KindRegistry instance;
			return instance;
		}

		void SetMetadata(std::type_index type, std::unique_ptr<KindInfo> metadata) {
			// We assert here because it is very bad practice to modify the reflection data after engine initalization.
			ASSERT(this->Instance().registeredTypes.find(type) == this->Instance().registeredTypes.end(), "Overwriting reflected type information is not allowed.")
				this->Instance().registeredTypes[type] = *metadata.get();
		};

		KindInfoRegistryEntry GetMetadata(std::type_index type)
		{
			return &this->Instance().registeredTypes.at(type);
		};

	private:
		std::unordered_map<std::type_index, KindInfo> registeredTypes;
	};

	void KindRegistryHandle::SetMetadata(std::type_index type, std::unique_ptr<KindInfo> metadata)
	{
		impl->SetMetadata(type, std::move(metadata));
	}

	KindInfoRegistryEntry KindRegistryHandle::GetMetadata(std::type_index type)
	{
		return impl->GetMetadata(type);
	}

	KindRegistryHandle KindRegistrar::registryHandle = KindRegistryHandle();
}