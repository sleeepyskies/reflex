#pragma once

#include <ranges>
#include <unordered_map>
#include "string_hash.hpp"
#include "field.hpp"


namespace reflex
{

/**
 * @brief A container for all the information associated with a type.
 */
class type_info
{
public:
    explicit type_info(const string_hash& hash) : m_hash(hash) {}

    /**
     * @brief Sets the hash value of this TypeInfo.
     * @param hash The hash to set.
     */
    void set_hash(const string_hash& hash) noexcept { m_hash = hash; }

    /**
     * @brief Returns the hash of this TypeInfo.
     * @return The hash of this TypeInfo.
     */
    [[nodiscard]] auto get_hash() const noexcept -> string_hash { return m_hash; }

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
    void insert_field(const string_hash& field_hash, const field& field) noexcept { m_fields.insert_or_assign(field_hash, field); }

    /**
     * @brief Finds and returns the Field associated with the given field name.
     * @param field_name The name of the type to lookup.
     * @return The Field associated with the given name.
     * @throws std::out_of_range If the field does not exist on this type.
     */
    [[nodiscard]] auto lookup_field(const std::string_view field_name) -> field& { return m_fields.at(string_hash{field_name}); }

    /**
     * @brief Returns the underlying map of StringHash to Field.
     * @return The map of StringHash to Field.
     */
    [[nodiscard]] auto fields() const noexcept -> auto { return std::views::values(m_fields); }

private:
    /// @brief The hash and the name of this TypeInfo.
    string_hash m_hash;
    /// @brief All fields captured in this TypeInfo.
    std::unordered_map<string_hash, field> m_fields;
};

} // namespace reflex
