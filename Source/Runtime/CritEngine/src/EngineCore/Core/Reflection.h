#pragma once
#include <string>
#include <memory>
#include <vector>
#include <typeindex>
#include <functional>
#include <any>

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

    template<typename Tuple, typename F, std::size_t... I>
    static void apply_with_index_impl(F&& f, Tuple& t, std::index_sequence<I...>)
    {
        (..., f(I, std::get<I>(t)));
    }

    template<typename Tuple, typename F>
    static void apply_with_index(F&& f, Tuple& t)
    {
        constexpr auto size = std::tuple_size_v<std::remove_reference_t<Tuple>>;
        apply_with_index_impl(std::forward<F>(f), t, std::make_index_sequence<size>{});
    }

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
            std::function<std::any(ClassType*, const std::vector<std::shared_ptr<void>>&)> invoker =
                [this, method](ClassType* instance, const std::vector<std::shared_ptr<void >>& args) -> std::any
            {
                ClassType* obj = static_cast<ClassType*>(instance);
                return this->MethodCallHelper(obj, method, args, std::index_sequence_for<Args...>{});
            };

            this->methods.emplace_back(
                Method(name,
                invoker,
                typeid(Ret),
                { typeid(Args)... })
            );
        }

        template<typename Class, typename Ret, typename... Args, std::size_t... I>
        Ret MethodCallHelper(ClassType* obj, Ret(Class::* method)(Args...),
                              const std::vector<std::shared_ptr<void>>& args, std::index_sequence<I...>)
        {
            return (obj->*method)(*(std::static_pointer_cast<Args>( args[I] ).get())...);
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
            std::function<std::any(ClassType*, const std::vector<std::shared_ptr<void>>&)> invoker;
            std::type_index returnType;
            std::vector<std::type_index> paramTypes;

            Method(std::string methodName, std::function<std::any(ClassType*, const std::vector<std::shared_ptr<void>>&)> invokeMethod, std::type_index methodReturnType, std::vector<std::type_index> methodParameters)
                : name(std::move(methodName)), invoker(invokeMethod), returnType(methodReturnType), paramTypes(methodParameters)
            {
            }
            virtual ~Method() = default;

            const std::string& GetName() const { return name; }

            template<typename... Args>
            std::any Call(ClassType* instance, Args&&... args) const
            {
                auto values = std::make_shared<std::tuple<std::decay_t<Args>...>>(std::tuple<std::decay_t<Args>...>(args)...);

                std::vector<std::shared_ptr<void>> argPtrs;
                apply_with_index([&](auto index, auto& elem)
                {
                    if (index < this->paramTypes.size() && this->paramTypes[index] != typeid(elem))
                    {
                        throw std::runtime_error("Invalid Parameter Signature was given to TypeInfo::Method::Call()");
                    }
                    else
                    {
                        argPtrs.push_back(std::make_shared<std::decay_t<decltype(elem)>>(elem));
                    }
                }, *values);

                return this->invoker(instance, argPtrs);
            }
        };

        std::string name;
        std::vector<Property> properties;
        std::vector<Method> methods;

    };

}