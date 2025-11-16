#pragma once

#include <ranges>
#include <unordered_map>
#include "hash.hpp"
#include "field.hpp"


namespace reflex
{

/**
 * @brief A container for all the information associated with a type.
 */
class type_info
{
public:
    explicit type_info(const type_hash& hash) noexcept : m_hash(hash) { }

    /**
     * @brief Sets the hash value of this TypeInfo.
     * @param hash The hash to set.
     */
    void set_hash(const type_hash& hash) noexcept { m_hash = hash; }

    /**
     * @brief Returns the hash of this TypeInfo.
     * @return The hash of this TypeInfo.
     */
    [[nodiscard]] auto get_hash() const noexcept -> type_hash { return m_hash; }

    /**
     * @brief Returns the name of this TypeInfo.
     * @return The unhashed string of this TypeInfo, aka the name of this type.
     */
    [[nodiscard]] auto name() const noexcept -> std::string_view { return m_hash.name(); }

    /**
     * @brief Inserts a new field into this TypeInfo. If the field hash already exists, it will be overwritten.
     * @param field_hash The name of the field to insert.
     * @param field The field to insert.
     */
    void insert_field(const name_hash& field_hash, const field& field) noexcept
    {
        m_fields.insert_or_assign(field_hash, field);
    }

    /**
     * @brief Finds and returns the field associated with the given field name.
     * @param field_name The name of the type to lookup.
     * @return The field associated with the given name.
     * @throws reflection_error If the field does not exist on this type.
     */
    [[nodiscard]] auto lookup_field(const std::string_view field_name) -> field&
    {
        const name_hash hash{ field_name };
        const auto it = m_fields.find(hash);
        if (it == m_fields.end()) { throw reflection_error{ "Field has not been captured." }; }
        return it->second;
    }

    /**
     * @brief Returns a view into the fields of this type.
     * @return A view into the fields of this type.
     */
    [[nodiscard]] auto fields() const noexcept -> auto { return std::views::values(m_fields); }

    /**
     * @brief Returns a view into the fields of this type.
     * @return A view into the fields of this type.
     */
    [[nodiscard]] auto fields() noexcept -> auto { return std::views::values(m_fields); }

    /**
     * @brief Applies a function to each captured field of this type.
     * @param fn The function to apply to each field.
     * @todo not done
     */
    void for_each_field(const std::function<void(field&)>& fn) { for (auto& field : fields()) { fn(field); } }

private:
    /// @brief The hash and the name of this type.
    type_hash m_hash;
    /// @brief All fields captured in this type.
    std::unordered_map<name_hash, field> m_fields;
};

} // namespace reflex
