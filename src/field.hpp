#pragma once

#include <functional>
#include <any>

#include "hash.hpp"
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
    [[nodiscard]] auto hash() const noexcept -> name_hash { return m_name_hash; }

    /**
     * @brief Returns the name of the field.
     * @return The name of the field.
     */
    [[nodiscard]] auto name() const noexcept -> std::string_view { return m_name_hash.name(); }

    /**
     * @brief Checks whether this field is static.
     * @return true if the field is static, false otherwise.
     */
    [[nodiscard]] constexpr auto is_static() const noexcept -> bool { return m_is_static; }

    /**
     * @brief Checks whether this field is const.
     * @return true if the field is const, false otherwise.
     */
    [[nodiscard]] constexpr auto is_const() const noexcept -> bool { return m_is_const; }

    /**
     * @brief Returns value of a field as std::any given an instance of the class. The caller must know the field type.
     * @tparam Class The class the field belongs to.
     * @param obj The object to get the field for.
     * @return A std::any holding the value of the field.
     * @remark @code int value = std::any_cast<int>(field.get(obj)); @endcode
     */
    template <typename Class>
    [[nodiscard]] auto get(Class& obj) const -> std::any { return m_get(static_cast<void*>(&obj)); }
    template <typename Class>
    [[nodiscard]] auto get(const Class& obj) const -> std::any { return m_get(static_cast<const void*>(&obj)); }

    /**
     * @brief Returns value of a field as the requested type given an instance of the class. The caller must know the field type.
     * @tparam T The type of the field to retrieve.
     * @tparam Class The class the field belongs to.
     * @param obj The object to get the field for.
     * @return The value of the field cast to T.
     * @remark @code int value = field.get<int>(obj); @endcode
     */
    template <typename T, typename Class>
    [[nodiscard]] auto get(Class& obj) const -> T { return std::any_cast<T>(m_get(static_cast<void*>(&obj))); }
    template <typename T, typename Class>
    [[nodiscard]] auto get(const Class& obj) const -> T { return std::any_cast<T>(m_get(static_cast<const void*>(&obj))); }

    /**
     * @brief Returns the type hash for this field.
     */
    [[nodiscard]] auto type() const noexcept -> type_hash { return m_type_hash; }

    /**
     * @brief Sets this field's value.
     * @tparam Class The class the field belongs to.
     * @tparam T The type of the field.
     * @param obj The object to set the field for.
     * @param value The value to set the field to.
     * @throw const_field_error if trying to set a const field.
     * @warning The caller must make sure that T matches the field's actual type.
     */
    template <typename Class, typename T>
    void set(Class& obj, const T& value) const { m_set(static_cast<void*>(&obj), std::any{value}); }

    /**
     * @brief Constructs a new member field object.
     * @tparam Class The class the field belongs to.
     * @tparam T The type of the field.
     * @param name_hash The hash of the field name.
     * @param type_hash The hash of the field type.
     * @param memberPtr The member pointer to the field.
     * @return A new field instance for a member field.
     */
    template <typename Class, typename T>
    static auto create_member(
        const name_hash& name_hash,
        const type_hash& type_hash,
        T Class::* memberPtr
    ) noexcept -> field
    {
        field f;
        f.m_name_hash = name_hash;
        f.m_type_hash = type_hash;
        f.m_is_static = false;
        f.m_is_const  = std::is_const_v<T>;

        f.m_get = [memberPtr] (const void* obj) -> std::any {
            auto* c = static_cast<const Class*>(obj);
            return std::any(c->*memberPtr);
        };

        if constexpr (std::is_const_v<T>) {
            f.m_set = [] (void*, const std::any&) { throw const_field_error{ }; };
        } else {
            f.m_set = [memberPtr] (void* obj, const std::any& value) {
                static_cast<Class*>(obj)->*memberPtr = std::any_cast<T>(value);
            };
        }

        return f;
    }

    /**
     * @brief Constructs a new static field object.
     * @tparam T The type of the field.
     * @param name_hash The hash of the field name.
     * @param type_hash The hash of the field type.
     * @param staticPtr The pointer to the field.
     * @return A new field instance for a static field.
     */
    template <typename T>
    static auto create_static(
        const name_hash& name_hash,
        const type_hash& type_hash,
        T* staticPtr
    ) noexcept -> field
    {
        field f;
        f.m_name_hash = name_hash;
        f.m_type_hash = type_hash;
        f.m_is_static = true;
        f.m_is_const  = std::is_const_v<T>;

        f.m_get = [staticPtr] (const void*) -> std::any {
            return *staticPtr;
        };

        if constexpr (std::is_const_v<T>) {
            f.m_set = [] (void*, const std::any&) {
                throw const_field_error{ };
            };
        } else {
            f.m_set = [staticPtr] (void*, const std::any& value) {
                *staticPtr = std::any_cast<T>(value);
            };
        }

        return f;
    }

private:
    /// @brief The name_hash of the field name.
    name_hash m_name_hash;
    /// @brief The type_hash of the field type.
    type_hash m_type_hash;

    /// @brief Whether the field is static.
    bool m_is_static = false;
    /// @brief Whether the field is const.
    bool m_is_const = false;

    /// @brief A function returning the value of the field given an instance.
    std::function<std::any(const void*)> m_get;
    /// @brief A function to set the value of the field given an instance.
    std::function<void(void*, const std::any&)> m_set;
};

} // namespace reflex
