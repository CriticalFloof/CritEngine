#pragma once
#include "Entity.h"
#include "Component.h"
#include "System.h"

#include <bitset>
#include <vector>
#include <memory>


namespace ECS
{
    static constexpr int MAX_COMPONENTS = 32;
    static constexpr int MAX_ENTITIES = 65536;

    using ComponentMask = std::bitset<MAX_COMPONENTS>;

    class Context
    {
    public:
        struct EntityDescription
        {
            EntityID id;
            ComponentMask mask;
        };

        EntityID createEntity()
        {
            if (!m_freeEntities.empty())
            {
                EntityIndex newIndex = m_freeEntities.back();
                m_freeEntities.pop_back();
                EntityID newID = EntityUtils::createEntityId(
                    newIndex, EntityUtils::getEntityVersion(m_entities[newIndex].id));
                m_entities[newIndex].id = newID;
                return m_entities[newIndex].id;
            }


            this->m_entities.push_back({
                EntityUtils::createEntityId(static_cast<EntityIndex>(m_entities.size()), 0), ComponentMask()
            });
            return m_entities.back().id;
        }

        void destroyEntity(EntityID id)
        {
            EntityID newID = EntityUtils::createEntityId(static_cast<EntityIndex>(UINT32_MAX),
                                                         EntityUtils::getEntityVersion(id) + 1);

            m_entities[EntityUtils::getEntityIndex(id)].id = newID;
            m_entities[EntityUtils::getEntityIndex(id)].mask.reset();

            m_freeEntities.push_back(EntityUtils::getEntityIndex(id));
        }

        EntityDescription& getEntity(EntityID id)
        {
            return this->m_entities[id];
        }

        size_t size()
        {
            return this->m_entities.size();
        }

        template <typename T>
        T* assign(EntityID id)
        {
            size_t componentId = getId<T>();

            if (this->m_componentPools.size() <= componentId)
            {
                this->m_componentPools.resize(componentId + 1);
            }
            if (this->m_componentPools[componentId] == nullptr)
            {
                this->m_componentPools[componentId] = std::make_unique<ComponentMemoryPool>(
                    sizeof(T), MAX_ENTITIES, alignof(T));
            }

            T* component = new(this->m_componentPools[componentId]->get(EntityUtils::getEntityIndex(id))) T();

            m_entities[id].mask.set(componentId, true);

            return component;
        }

        template <typename T>
        T* getComponent(EntityID id)
        {
            int componentId = getId<T>();
            if (!m_entities[id].mask.test(componentId))
                return nullptr;

            T* component = static_cast<T*>(this->m_componentPools[componentId]->get(EntityUtils::getEntityIndex(id)));
            return component;
        }

        template <typename T>
        void unassign(EntityID id)
        {
            if (m_entities[EntityUtils::getEntityIndex(id)].id != id) return;

            int componentId = getId<T>();
            m_entities[id].mask.set(componentId, false);
        }

    private:
        std::vector<EntityIndex> m_freeEntities;
        std::vector<EntityDescription> m_entities;
        std::vector<std::unique_ptr<ComponentMemoryPool>> m_componentPools;
    };

    template <typename... ComponentTypes>
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
                int component_ids[] = {0, getId<ComponentTypes>()...};
                for (size_t i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
                {
                    this->componentMask.set(component_ids[i], true);
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
                return this->context->getEntity(this->index).id;
            }

            bool operator==(const Iterator& other) const
            {
                return this->index == other.index || this->index == this->context->size();
            }

            bool operator!=(const Iterator& other) const
            {
                return this->index != other.index && this->index != this->context->size();
            }

            Iterator& operator++()
            {
                do
                {
                    index++;
                }
                while (index < this->context->size() && !validIndex());
                return *this;
            }

            bool validIndex()
            {
                return
                (
                    EntityUtils::isEntityValid(this->context->getEntity(this->index).id) &&
                    (this->all || this->mask == (this->mask & this->context->getEntity(this->index).mask))
                );
            }

            EntityIndex index;
            Context* context;
            ComponentMask mask;
            bool all{false};
        };

        const Iterator begin() const
        {
            size_t first_index = 0;
            while (first_index < this->context->size() &&
                (this->componentMask != (this->componentMask & this->context->getEntity(first_index).mask)
                    || !EntityUtils::isEntityValid(this->context->getEntity(first_index).id)))
            {
                first_index++;
            }
            return Iterator(this->context, static_cast<EntityIndex>(first_index), this->componentMask, this->all);
        }

        const Iterator end() const
        {
            return Iterator(this->context, static_cast<EntityIndex>(this->context->size()), this->componentMask,
                            this->all);
        }

        Context* context{nullptr};
        ComponentMask componentMask;
        bool all{false};
    };
}
