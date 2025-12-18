#pragma once

#include "context.hpp"
#include "descriptor.hpp"
#include "range.hpp"


namespace reflex
{
class type_handle;
class field_handle;

using field_range = range<field_handle, internal::field_descriptor>;

class type_handle
{
public:
    type_handle(context* ctx, internal::type_descriptor* inner) : m_ctx(ctx), m_inner(inner) { }

    auto name() const -> const char* { return m_inner->hash.data(); }

    auto fields() const -> field_range
    {
        return field_range{
            m_ctx,
            m_inner->fields.data(),
            m_inner->fields.size(),
        };
    }

private:
    context* m_ctx;
    internal::type_descriptor* m_inner;
};

class field_handle
{
public:
    field_handle(context* ctx, internal::field_descriptor* inner) : m_ctx(ctx), m_inner(inner) { }

    auto name() const -> const char* { return m_inner->field_hash.data(); }

    auto type() const -> type_handle
    {
        return type_handle{
            m_ctx,
            &m_ctx->at(m_inner->type_hash)
        };
    }

    auto attribute(const char* key) const -> std::any { return m_inner->attributes.at(hashed_string{ key }); }

private:
    context* m_ctx;
    internal::field_descriptor* m_inner;
};
}
