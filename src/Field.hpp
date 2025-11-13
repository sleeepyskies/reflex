#pragma once

#include <functional>
#include <stdexcept>

#include "Alias.hpp"


namespace reflex
{

/**
 * @brief A representation of an object field.
 */
class Field
{
public:
    /**
     * @brief Checks whether this field is static.
     * @return true if the field is static, false otherwise.
     */
    [[nodiscard]] auto is_static() const noexcept -> bool { return m_is_static; }

    /**
     * @brief Checks whether this field is const.
     * @return true if the field is const, false otherwise.
     */
    [[nodiscard]] auto is_const() const noexcept -> bool { return m_is_const; }

    /**
     * @brief
     * @tparam Class The class the field belongs to.
     * @param obj The object to set the field for.
     * @return A const void* or void* to the value of the field.
     * @remark The result can be cast like: `auto& value = *static_cast<int*>(field.get());`
     */
    template <typename Class>
    [[nodiscard]] auto get(Class& obj) const noexcept -> auto*
    {
        if constexpr (!std::is_const_v<Class>) {
            return m_get(static_cast<void*>(&obj));
        } else {
            // cast away constness, then cast it back
            return static_cast<const void*>(m_get(const_cast<void*>(static_cast<const void*>(&obj))));
        }
    }

    /**
     * @brief Sets this fields value.
     * @tparam Class The class the field belongs to.
     * @tparam T The type of the field.
     * @param obj The object to set the field for.
     * @param value The value to set the field to.
     */
    template <typename Class, typename T>
    void set(Class& obj, const T& value) const noexcept
    {
        m_set(static_cast<void*>(&obj), static_cast<const void*>(&value));
    }

    /**
     * @brief Constructs a new member field object.
     * @tparam Class The class the field belongs to.
     * @tparam T The type of the field.
     * @param memberPtr The member pointer to the field.
     * @return A new Field instance for a member field.
     */
    template <typename Class, typename T>
    static auto create_member(T Class::* memberPtr) -> Field
    {
        Field f;
        f.m_is_static = false;
        f.m_is_const  = std::is_const_v<T>;

        f.m_get = [memberPtr] (void* obj) -> void* {
            return &(static_cast<Class*>(obj)->*memberPtr);
        };

        if constexpr (std::is_const_v<T>) {
            f.m_set = [] (void*, const void*) {
                throw std::runtime_error("Cannot set const field");
            };
        } else {
            f.m_set = [memberPtr] (void* obj, const void* value) {
                static_cast<Class*>(obj)->*memberPtr = *static_cast<const T*>(value);
            };
        }

        return f;
    }

    /**
     * @brief Constructs a new static field object.
     * @tparam T The type of the field.
     * @param staticPtr The pointer to the field.
     * @return A new Field instance for a static field.
     */
    template <typename T>
    static auto create_static(T* staticPtr) -> Field
    {
        Field f;
        f.m_is_static = true;
        f.m_is_const  = std::is_const_v<T>;

        f.m_get = [staticPtr] (void*) -> void* {
            return staticPtr;
        };

        if constexpr (std::is_const_v<T>) {
            f.m_set = [] (void*, const void*) {
                throw std::runtime_error("Cannot set const static field");
            };
        } else {
            f.m_set = [staticPtr] (void*, const void* value) {
                *staticPtr = *static_cast<const T*>(value);
            };
        }

        return f;
    }

private:
    /// @brief Whether the field is static.
    bool m_is_static = false;
    /// @brief Whether the field is const.
    bool m_is_const = false;

    /// @brief A function returning the address of the field given an instance.
    std::function<void*(void*)> m_get;
    /// @brief A function to set the value of the field given an instance.
    std::function<void(void*, const void*)> m_set;
};

} // namespace reflex
