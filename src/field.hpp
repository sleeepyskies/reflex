#pragma once

#include <functional>
#include <stdexcept>
#include <any>

#include "alias.hpp"
#include "exception.hpp"


namespace reflex
{

/**
 * @brief A representation of an object field.
 */
class field
{
public:
    /**
     * @brief Returns the string_hash of this field.
     * @return The string_hash of this field.
     */
    [[nodiscard]] auto hash() const noexcept -> string_hash { return m_hash; }

    /**
     * @brief Returns the name of the field.
     * @return The name of the field.
     */
    [[nodiscard]] auto name() const noexcept -> std::string_view { return m_hash.name(); }

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
     * @brief Returns value of a field as void* given an instance of the class. The caller must know the field type.
     * @tparam Class The class the field belongs to.
     * @param obj The object to set the field for.
     * @return A const void* or void* to the value of the field.
     * @remark @code int& value = *static_cast<int*>(field.get()); @endcode
     */
    template <typename Class>
    [[nodiscard]] auto get(Class& obj) const noexcept -> std::any
    {
        return m_get(static_cast<void*>(&obj));
    }

    template <typename T, typename Class>
    [[nodiscard]] auto get(Class& obj) const noexcept -> T
    {
        return std::any_cast<T>(m_get(static_cast<void*>(&obj)));
    }

    /**
     * @brief Sets this fields value.
     * @tparam Class The class the field belongs to.
     * @tparam T The type of the field.
     * @param obj The object to set the field for.
     * @param value The value to set the field to.
     * @warning The caller must make sure that T matches the fields actual type.
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
     * @param hash The hash of the field.
     * @param memberPtr The member pointer to the field.
     * @return A new field instance for a member field.
     */
    template <typename Class, typename T>
    static auto create_member(const string_hash& hash, T Class::* memberPtr) -> field
    {
        field f;
        f.m_hash      = hash;
        f.m_is_static = false;
        f.m_is_const  = std::is_const_v<T>;

        f.m_get = [memberPtr] (void* obj) -> std::any {
            auto* c = static_cast<Class*>(obj);
            return std::any(c->*memberPtr);
        };

        if constexpr (std::is_const_v<T>) {
            f.m_set = [] (void*, const std::any&) {
                throw const_field_error{ };
            };
        } else {
            f.m_set = [memberPtr] (void* obj, const std::any& value) {
                static_cast<Class*>(obj)->*memberPtr = *std::any_cast<const T*>(value);
            };
        }

        return f;
    }

    /**
     * @brief Constructs a new static field object.
     * @tparam T The type of the field.
     * @param hash The hash of the field.
     * @param staticPtr The pointer to the field.
     * @return A new field instance for a static field.
     */
    template <typename T>
    static auto create_static(const string_hash& hash, T* staticPtr) -> field
    {
        field f;
        f.m_hash      = hash;
        f.m_is_static = true;
        f.m_is_const  = std::is_const_v<T>;

        f.m_get = [staticPtr] (void*) -> std::any {
            return staticPtr;
        };

        if constexpr (std::is_const_v<T>) {
            f.m_set = [] (void*, const void*) {
                throw const_field_error{ };
            };
        } else {
            f.m_set = [staticPtr] (void*, const void* value) {
                *staticPtr = *static_cast<const T*>(value);
            };
        }

        return f;
    }

private:
    /// @brief The string_hash of the field.
    string_hash m_hash;

    /// @brief Whether the field is static.
    bool m_is_static = false;
    /// @brief Whether the field is const.
    bool m_is_const = false;

    /// @brief A function returning the address of the field given an instance.
    std::function<std::any(void*)> m_get;
    /// @brief A function to set the value of the field given an instance.
    std::function<void(void*, std::any)> m_set;
};

} // namespace reflex
