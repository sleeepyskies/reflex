#pragma once

#include <string_view>

#include "Context.hpp"
#include "Field.hpp"
#include "StringHash.hpp"
#include "traits.hpp"


namespace reflex
{



template <typename T>
struct TypeBuilder
{
    explicit TypeBuilder(Context& ctx) : m_ctx(&ctx) { }

    auto type(StringHash& hash) -> TypeBuilder&
    {
        auto& ctx = m_ctx->ctx;
        if (ctx.contains(hash)) { ctx[hash].setHash(hash); }
        else { m_ctx->ctx[hash] = TypeInfo{}; }
        m_alias = &hash;
        return *this;
    }

    template <auto Ptr>
        requires MemberFieldPointer<Ptr>
    auto field(const std::string_view alias) -> TypeBuilder&
    {
        auto& ctx = m_ctx->ctx;
        ctx[*m_alias].insert_field(StringHash{alias}, Field::create_member(Ptr));
        return *this;
    }

    template <auto Ptr>
        requires StaticFieldPointer<Ptr>
    auto field(const std::string_view alias) -> TypeBuilder&
    {
        auto& ctx = m_ctx->ctx;
        ctx[*m_alias].insert_field(StringHash{alias}, Field::create_static(Ptr));
        return *this;
    }

private:
    Context* m_ctx;
    StringHash* m_alias = nullptr;
};


namespace internal
{
/**
 * @brief A helper struct holding a global reflex context.
 */
struct Global
{
    /// @brief The global reflex context.
    static inline Context ctx;
};
}


template <typename T>
auto capture(Context& ctx, const std::string_view alias) -> TypeBuilder<T>
{
    return TypeBuilder<T>(ctx).type(Alias<T>{ alias }.hash);
}

template <typename T>
auto capture(const std::string_view alias) -> TypeBuilder<T>
{
    return TypeBuilder<T>(internal::Global::ctx).type(Alias<T>{ alias }.hash);
}

template <typename T>
auto lookup() -> TypeInfo
{
    return internal::Global::ctx.ctx[Alias<T>::hash];
}

template <typename T>
auto lookup(const Context& ctx) -> TypeInfo
{
    return ctx.ctx[Alias<T>::alias];
}

inline auto lookup(const std::string_view name) -> TypeInfo
{
    return internal::Global::ctx.ctx[StringHash{name}];
}

inline auto lookup(const Context& ctx, const std::string_view name) -> TypeInfo
{
    return ctx.ctx.at(StringHash{name});
}

} // namespace reflex
