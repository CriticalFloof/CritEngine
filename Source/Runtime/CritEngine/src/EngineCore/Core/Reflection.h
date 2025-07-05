#pragma once
#include <string>
#include <memory>
#include <vector>
#include <typeindex>

#define REFLECT(CLASS_NAME) \
    static TypeInfo<CLASS_NAME>& GetTypeInfo() { \
        static TypeInfo<CLASS_NAME> info{#CLASS_NAME}; \
        return info; \
    }

#define PROPERTY(CLASS_NAME, FIELD_TYPE, FIELD_NAME) \
    namespace { \
        struct __PropertyRegister_##CLASS_NAME##_##FIELD_NAME { \
            __PropertyRegister_##CLASS_NAME##_##FIELD_NAME() { \
                CLASS_NAME::GetTypeInfo().AddProperty<FIELD_TYPE>(#FIELD_NAME, offsetof(CLASS_NAME, FIELD_NAME)); \
            } \
        }; \
        static __PropertyRegister_##CLASS_NAME##_##FIELD_NAME __property_instance_##CLASS_NAME##_##FIELD_NAME; \
    }

namespace Engine {

    template <typename ClassType, typename MemberType>
    constexpr size_t offset_of(MemberType ClassType::* member)
    {
        return reinterpret_cast<size_t>(
            reinterpret_cast<char const volatile*>(&(reinterpret_cast<ClassType const volatile*>(0)->*member))
            );
    }

    template<typename ClassType>
    struct TypeInfo
    {
        std::string name;
        TypeInfo(std::string name) 
            : name(name)
        {
        }
        virtual ~TypeInfo() = default;

        struct Property
        {
            std::string name;
            size_t offset;
            std::type_index type;

            Property(std::string n, size_t o, std::type_index t) : name(std::move(n)), offset(o), type(t) {}
            virtual ~Property() = default;

            const std::string& getName() const { return name; }

            void* get(ClassType& instance) const
            {
                return reinterpret_cast<char*>(reinterpret_cast<void*>(&instance)) + offset;
            }

            template<typename FieldType>
            void set(ClassType& instance, void* value) const
            {
                *reinterpret_cast<FieldType*>(
                    reinterpret_cast<char*>(reinterpret_cast<void*>(&instance)) + offset
                    ) = *reinterpret_cast<FieldType*>(value);
            }
        };

        std::vector<Property> properties;

        template<typename FieldType>
        void AddProperty(const std::string& name, size_t offset)
        {
            properties.emplace_back(Property(name, offset, typeid(FieldType)));
        }
    };

}