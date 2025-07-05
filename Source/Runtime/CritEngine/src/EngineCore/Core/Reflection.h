#pragma once
#include <string>
#include <memory>
#include <vector>
#include <typeindex>
#include <functional>

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

#define METHOD(CLASS_NAME, METHOD_NAME) \
    namespace { \
        struct __MethodRegister_##CLASS_NAME##_##METHOD_NAME { \
            __MethodRegister_##CLASS_NAME##_##METHOD_NAME() { \
                CLASS_NAME::GetTypeInfo().AddMethod(#METHOD_NAME, &CLASS_NAME::METHOD_NAME); \
            } \
        }; \
        static __MethodRegister_##CLASS_NAME##_##METHOD_NAME __method_instance_##CLASS_NAME##_##METHOD_NAME; \
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

            Property(std::string propertyName, size_t propertyOffset, std::type_index propertyType) 
                : name(std::move(propertyName)), offset(propertyOffset), type(propertyType) {}
            virtual ~Property() = default;

            const std::string& GetName() const { return name; }

            void* Get(ClassType& instance) const
            {
                return reinterpret_cast<char*>(reinterpret_cast<void*>(&instance)) + offset;
            }

            template<typename FieldType>
            void Set(ClassType& instance, void* value) const
            {
                *reinterpret_cast<FieldType*>(
                    reinterpret_cast<char*>(reinterpret_cast<void*>(&instance)) + offset
                    ) = *reinterpret_cast<FieldType*>(value);
            }
        };

        struct Method
        {
            std::string name;
            std::function<void(void*, const std::vector<void*>&)> invoker;
            std::type_index returnType;
            std::vector<std::type_index> paramTypes;
            
            Method(std::string methodName, std::function<void(void*, const std::vector<void*>&)> invokeMethod, std::type_index methodReturnType, std::vector<std::type_index> methodParameters)
                : name(std::move(methodName)), invoker(invokeMethod), returnType(methodReturnType), paramTypes(methodParameters) {}
            virtual ~Method() = default;

            const std::string& GetName() const { return name; }

            void Invoke(ClassType& instance) const { 
                this->invoker(static_cast<void*>(&instance), {});
            }
        };

        std::vector<Property> properties;
        std::vector<Method> methods;

        template<typename FieldType>
        void AddProperty(const std::string& name, size_t offset)
        {
            this->properties.emplace_back(Property(name, offset, typeid(FieldType)));
        }

        template<typename Class, typename Ret, typename... Args>
        void AddMethod(const std::string& name, Ret(Class::* method)(Args...))
        {
            std::function<void(void*, const std::vector<void*>&)> invoker =
                [this, method](void* instance, const std::vector<void*>& args)
            {
                Class* obj = static_cast<Class*>(instance);
                this->MethodCallHelper(obj, method, args, std::index_sequence_for<Args...>{});
            };

            this->methods.emplace_back(
                Method(name,
                invoker,
                typeid(Ret),
                { typeid(Args)... })
            );
        }

        template<typename Class, typename Ret, typename... Args, std::size_t... I>
        void MethodCallHelper(Class* obj, Ret(Class::* method)(Args...),
                              const std::vector<void*>& args, std::index_sequence<I...>)
        {
            (obj->*method)(*reinterpret_cast<std::remove_reference_t<Args>*>(args[I])...);
        }

        
    };

}