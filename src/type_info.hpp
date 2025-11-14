#pragma once

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
    type_info() = default;

    /**
     * @brief Sets the hash value of this TypeInfo.
     * @param hash The hash to set.
     */
    void setHash(const string_hash& hash) { m_hash = hash; }

    /**
     * @brief Returns the hash of this TypeInfo.
     * @return The hash of this TypeInfo.
     */
    auto getHash() const -> string_hash { return m_hash; }

    /**
     * @brief Returns the name of this TypeInfo.
     * @return The unhashed string of this TypeInfo, aka the name of this type.
     */
    auto name() const -> std::string_view { return m_hash.getString(); }

    /**
     * @brief Inserts a new field into this TypeInfo.
     * @param hash The name of the field to insert.
     * @param field The field to insert.
     */
    void insert_field(const string_hash& hash, const field& field) { m_fields.insert_or_assign(hash, field); }

    /**
     * @brief Finds and returns the Field associated with the given field name.
     * @param name The name of the type to lookup.
     * @return The Field associated with the given name.
     * @throws If the field does not exist on this type.
     */
    auto lookup_field(const std::string_view name) -> field& { return m_fields.at(string_hash{name}); }

    /**
     * @brief Returns the underlying map of StringHash to Field.
     * @return The map of StringHash to Field.
     */
    auto fields() const -> const std::unordered_map<string_hash, field>& { return m_fields; }

private:
    /// @brief The hash and the name of this TypeInfo.
    string_hash m_hash;
    /// @brief All fields captured in this TypeInfo.
    std::unordered_map<string_hash, field> m_fields;
};

} // namespace reflex
