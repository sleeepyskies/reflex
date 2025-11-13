#pragma once

#include <functional>
#include <unordered_map>
#include "StringHash.hpp"


namespace reflex
{

struct Field
{
    StringHash alias;
    bool isStatic = false;
    bool isConst  = false;

    std::function<void*(void*)> get;
    std::function<void(void*, const void*)> set;

    template <typename Class, typename T>
    static Field makeMember(const StringHash& name, T Class::* memberPtr)
    {
        Field f;
        f.alias    = name;
        f.isStatic = false;
        f.isConst  = std::is_const_v<T>;

        f.get = [memberPtr] (void* obj) -> void* {
            return &(static_cast<Class*>(obj)->*memberPtr);
        };

        if constexpr (!std::is_const_v<T>) {
            f.set = [memberPtr] (void* obj, const void* value) {
                static_cast<Class*>(obj)->*memberPtr = *static_cast<const T*>(value);
            };
        } else {
            f.set = [] (void*, const void*) {
                throw std::runtime_error("Cannot set const field");
            };
        }

        return f;
    }

    template <typename T>
    static Field makeStatic(const StringHash& name, T* staticPtr)
    {
        Field f;
        f.alias    = name;
        f.isStatic = true;
        f.isConst  = std::is_const_v<T>;

        f.get = [staticPtr] (void*) -> void* {
            return const_cast<T*>(staticPtr);
        };

        if constexpr (!std::is_const_v<T>) {
            f.set = [staticPtr] (void*, const void* value) {
                *staticPtr = *static_cast<const T*>(value);
            };
        } else {
            f.set = [] (void*, const void*) {
                throw std::runtime_error("Cannot set const static field");
            };
        }

        return f;
    }

};

struct TypeData
{
    std::unordered_map<StringHash, Field> fields;
};

struct Context
{
    std::unordered_map<StringHash, TypeData> ctx{ };
};

} // namespace reflex
