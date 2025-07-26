#pragma once
#include "Entity.h"
#include "Component.h"
#include "System.h"

#include <bitset>
#include <vector>
#include <memory>


namespace ECS {
	static const int MAX_COMPONENTS = 32;
	static const int MAX_ENTITIES = 65536;

	typedef std::bitset<MAX_COMPONENTS> ComponentMask;

	class Context
	{
	public:

		struct EntityDescription
		{
			EntityID id;
			ComponentMask mask;
		};

		EntityID CreateEntity()
		{
			if (!freeEntities.empty())
			{
				EntityIndex newIndex = freeEntities.back();
				freeEntities.pop_back();
				EntityID newID = EntityUtils::CreateEntityId(newIndex, EntityUtils::GetEntityVersion(entities[newIndex].id));
				entities[newIndex].id = newID;
				return entities[newIndex].id;
			}

			
			this->entities.push_back({ EntityUtils::CreateEntityId(static_cast<EntityIndex>(entities.size()), 0), ComponentMask() });
			return entities.back().id;
		}

		void DestroyEntity(EntityID id)
		{
			EntityID newID = EntityUtils::CreateEntityId(EntityIndex(UINT32_MAX), EntityUtils::GetEntityVersion(id) + 1);

			entities[EntityUtils::GetEntityIndex(id)].id = newID;
			entities[EntityUtils::GetEntityIndex(id)].mask.reset();

			freeEntities.push_back(EntityUtils::GetEntityIndex(id));
		}

		EntityDescription& GetEntity(EntityID id)
		{
			return this->entities[id];
		}

		size_t Size()
		{
			return this->entities.size();
		}

		template<typename T>
		T* Assign(EntityID id)
		{
			size_t componentId = GetId<T>();

			if (this->componentPools.size() <= componentId)
			{
				this->componentPools.resize(componentId + 1);
			}
			if (this->componentPools[componentId] == nullptr)
			{
				this->componentPools[componentId] = std::make_unique<ComponentMemoryPool>(sizeof(T), MAX_ENTITIES, alignof(T));
			}

			T* component = new (this->componentPools[componentId]->Get(EntityUtils::GetEntityIndex(id))) T();
			
			entities[id].mask.set(componentId, true);

			return component;
		}

		template<typename T>
		T* GetComponent(EntityID id)
		{
			int componentId = GetId<T>();
			if (!entities[id].mask.test(componentId))
				return nullptr;

			T* component = static_cast<T*>(this->componentPools[componentId]->Get(EntityUtils::GetEntityIndex(id)));
			return component;
		}

		template<typename T>
		void Unassign(EntityID id)
		{
			if (entities[EntityUtils::GetEntityIndex(id)].id != id) return;

			int componentId = GetId<T>();
			entities[id].mask.set(componentId, false);
		}

	private:
		std::vector<EntityIndex> freeEntities;
		std::vector<EntityDescription> entities;
		std::vector<std::unique_ptr<ComponentMemoryPool>> componentPools;
	};

	template<typename... ComponentTypes>
	struct ContextView
	{
		ContextView(Context& context) : context(&context)
		{
			if (sizeof...(ComponentTypes) == 0)
			{
				all = true;
			}
			else
			{
				// Unpack the template parameters into an initializer list
				int componentIds[] = { 0, GetId<ComponentTypes>() ... };
				for (size_t i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
				{
					this->componentMask.set(componentIds[i], true);
				}
			}
		}

		struct Iterator
		{
			Iterator(Context* context, EntityIndex index, ComponentMask mask, bool all)
				: index(index), context(context), mask(mask), all(all)
			{
			}

			EntityID operator*() const
			{
				return this->context->GetEntity(this->index).id;
			}

			bool operator==(const Iterator& other) const
			{
				return this->index == other.index || this->index == this->context->Size();
			}

			bool operator!=(const Iterator& other) const
			{
				return this->index != other.index && this->index != this->context->Size();
			}

			Iterator& operator++()
			{
				do
				{
					index++;
				} while (index < this->context->Size() && !ValidIndex());
				return *this;
			}

			bool ValidIndex()
			{
				return 
				(
					EntityUtils::IsEntityValid(this->context->GetEntity(this->index).id) &&
					(this->all || this->mask == (this->mask & this->context->GetEntity(this->index).mask))
				);
			}

			EntityIndex index;
			Context* context;
			ComponentMask mask;
			bool all { false };
		};

		const Iterator begin() const
		{
			size_t firstIndex = 0;
			while (firstIndex < this->context->Size() &&
				   (this->componentMask != (this->componentMask & this->context->GetEntity(firstIndex).mask)
				   || !EntityUtils::IsEntityValid(this->context->GetEntity(firstIndex).id)))
			{
				firstIndex++;
			}
			return Iterator(this->context, static_cast<EntityIndex>(firstIndex), this->componentMask, this->all);
		}

		const Iterator end() const
		{
			return Iterator(this->context, EntityIndex(this->context->Size()), this->componentMask, this->all);
		}

		Context* context { nullptr };
		ComponentMask componentMask;
		bool all { false };
	};

}