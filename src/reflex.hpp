#pragma once

#include <string_view>

#include "Context.hpp"
#include "StringHash.hpp"
#include "traits.hpp"


namespace reflex
{

/**
 * @brief Helper struct to create an alias between a StringHash and a type T.
 * @tparam T The type to create an alias for.
 * @remark Constructing a second Alias<T> will overwrite the previous alias.
 */
template <typename T>
struct Alias
{
    constexpr explicit Alias(const std::string_view a) { alias = StringHash{ a }; }

    static inline StringHash alias;
};

template <typename T>
struct TypeBuilder
{
    explicit TypeBuilder(Context& ctx) : m_ctx(&ctx) { }

    auto type(StringHash& alias) -> TypeBuilder&
    {
        m_ctx->ctx[alias] = TypeData{};
        m_alias = &alias;
        return *this;
    }

    template <auto Ptr>
        requires MemberFieldPointer<Ptr>
    auto field(const std::string_view alias) -> TypeBuilder&
    {
        m_ctx->ctx[*m_alias].fields[StringHash{alias}] = Field::makeMember(StringHash{alias}, Ptr);
        return *this;
    }

    template <auto Ptr>
        requires StaticFieldPointer<Ptr>
    auto field(const std::string_view alias) -> TypeBuilder&
    {
        m_ctx->ctx[*m_alias].fields[StringHash{alias}] = Field::makeStatic(StringHash{alias}, Ptr);
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
    return TypeBuilder<T>(ctx).type(Alias<T>{ alias }.alias);
}

template <typename T>
auto capture(const std::string_view alias) -> TypeBuilder<T>
{
    return TypeBuilder<T>(internal::Global::ctx).type(Alias<T>{ alias }.alias);
}

template <typename T>
auto lookup() -> TypeData
{
    return internal::Global::ctx.ctx[Alias<T>::alias];
}

template <typename T>
auto lookup(const Context& ctx) -> TypeData
{
    return ctx.ctx[Alias<T>::alias];
}

inline auto lookup(const std::string_view name) -> TypeData
{
    return internal::Global::ctx.ctx[StringHash{name}];
}

inline auto lookup(const Context& ctx, const std::string_view name) -> TypeData
{
    return ctx.ctx.at(StringHash{name});
}

} // namespace reflex
