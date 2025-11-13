#pragma once

#include <unordered_map>
#include "StringHash.hpp"
#include "Field.hpp"


namespace reflex
{

class TypeInfo
{
public:
    TypeInfo() = default;

    void setHash(const StringHash& hash) { m_hash = hash; }

    auto getHash() const -> StringHash { return m_hash; }

    auto name() const -> std::string_view { return m_hash.getString(); }

    void insert_field(const StringHash& hash, const Field& field) { m_fields.insert_or_assign(hash, field); }

    auto lookup_field(const std::string_view name) -> Field& { return m_fields.at(StringHash{name}); }

    auto fields() const -> const std::unordered_map<StringHash, Field>& { return m_fields; }

private:
    StringHash m_hash;
    std::unordered_map<StringHash, Field> m_fields;
};

struct Context
{
    std::unordered_map<StringHash, TypeInfo> ctx{ };
};

} // namespace reflex
