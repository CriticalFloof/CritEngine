#pragma once
#include <string>
#include <memory>
#include <vector>
#include <typeindex>
#include <functional>
#include <cstdio>

#define REFLECT(CLASS_NAME) \
    static TypeInfo<CLASS_NAME>& GetTypeInfo() { \
        static TypeInfo<CLASS_NAME> info{#CLASS_NAME}; \
        return info; \
    }

#define PROPERTY(CLASS_NAME, FIELD_NAME) \
    namespace { \
        struct __PropertyRegister_##CLASS_NAME##_##FIELD_NAME { \
            __PropertyRegister_##CLASS_NAME##_##FIELD_NAME() { \
                CLASS_NAME::GetTypeInfo().AddProperty(#FIELD_NAME, &CLASS_NAME::FIELD_NAME ); \
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

    template<typename ClassType>
    struct TypeInfo
    {

        TypeInfo(std::string name) 
            : name(name)
        {
        }
        virtual ~TypeInfo() = default;

        template<typename Class, typename PropertyType>
        void AddProperty(const std::string& name, PropertyType Class::* propertyPtr)
        {
            
            std::cout << typeid(PropertyType).name() << "\n";
            this->properties.emplace_back(Property(name, reinterpret_cast<void* Class::*>(propertyPtr), typeid(PropertyType)));
        }

        template<typename Class, typename Ret, typename... Args>
        void AddMethod(const std::string& name, Ret(Class::* method)(Args...))
        {
            std::function<void(ClassType*, const std::vector<void*>&)> invoker =
                [this, method](ClassType* instance, const std::vector<void*>& args)
            {
                ClassType* obj = static_cast<ClassType*>(instance);
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
        void MethodCallHelper(ClassType* obj, Ret(Class::* method)(Args...),
                              const std::vector<void*>& args, std::index_sequence<I...>)
        {
            (obj->*method)(*reinterpret_cast<std::remove_reference_t<Args>*>(args[I])...);
        }

        struct Property
        {
            std::string name;
            void* ClassType::* data;
            std::type_index type;

            Property(std::string propertyName, void* ClassType::* propertyData, std::type_index propertyType)
                : name(std::move(propertyName)), data(propertyData), type(propertyType)
            {
            }
            virtual ~Property() = default;

            const std::string& GetName() const { return name; }

            template<typename FieldType>
            FieldType* Get(ClassType* instance) const
            {
                if (this->type != typeid(FieldType))
                {
                    return nullptr;
                }

                return reinterpret_cast<FieldType*>(&(instance->*data));
            }

            template<typename FieldType>
            bool Set(ClassType* instance, FieldType value) const
            {
                if (this->type != typeid(value))
                {
                    return false;
                }

                *reinterpret_cast<FieldType*>(&(instance->*data)) = value;
                return true;
            }
        };

        struct Method
        {
            std::string name;
            std::function<void(ClassType*, const std::vector<void*>&)> invoker;
            std::type_index returnType;
            std::vector<std::type_index> paramTypes;

            Method(std::string methodName, std::function<void(ClassType*, const std::vector<void*>&)> invokeMethod, std::type_index methodReturnType, std::vector<std::type_index> methodParameters)
                : name(std::move(methodName)), invoker(invokeMethod), returnType(methodReturnType), paramTypes(methodParameters)
            {
            }
            virtual ~Method() = default;

            const std::string& GetName() const { return name; }

            void Invoke(ClassType* instance) const
            {
                this->invoker(instance, {});
            }
        };

        std::string name;
        std::vector<Property> properties;
        std::vector<Method> methods;

    };

}