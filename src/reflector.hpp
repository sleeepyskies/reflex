#pragma once

#include <string_view>

#include "type_info.hpp"
#include "field.hpp"
#include "string_hash.hpp"
#include "traits.hpp"


namespace reflex
{

/// @brief A Storage container for reflected types.
using context = std::unordered_map<string_hash, type_info>;


namespace internal
{
/**
 * @brief A helper struct holding a global reflex context.
 */
struct global
{
    /// @brief The global reflex context.
    static inline context ctx;
};
}


/**
 * @brief Builds and registers type metadata. Used for constructing type_info.
 * @tparam T The reflected type.
 */
template <typename T>
struct reflector
{
    /**
     * @brief Creates a Reflector bound to a context.
     * @param ctx Reflection context.
     * @param hash The string_hash to associate with T
     */
    explicit reflector(context& ctx, const string_hash& hash) : m_ctx(&ctx), m_type_hash(hash)
    {
        m_ctx->try_emplace(m_type_hash, type_info{m_type_hash});
    }

    /**
     * @brief Captures a member field entry.
     * @tparam Ptr The member pointer of the field.
     * @param field_name The name of the field.
     * @return A reference to the reflector for use in a builder pattern.
     */
    template <auto Ptr>
        requires member_field_ptr<Ptr>
    auto field(const std::string_view field_name) -> reflector&
    {
        auto& ctx = *m_ctx;
        const string_hash field_hash{field_name};
        ctx.at(m_type_hash).insert_field(field_hash, field::create_member(field_hash, Ptr));
        return *this;
    }

    /**
     * @brief Captures a static field entry.
     * @tparam Ptr The static pointer to the field.
     * @param field_name The name of the field.
     * @return A reference to the Reflector for use in a builder pattern.
     */
    template <auto Ptr>
        requires static_field_ptr<Ptr>
    auto field(const std::string_view field_name) -> reflector&
    {
        auto& ctx = *m_ctx;
        const string_hash field_hash{field_name};
        ctx.at(m_type_hash).insert_field(field_hash, field::create_static(field_hash, Ptr));
        return *this;
    }

private:
    /// @brief The context to capture into.
    context* m_ctx;
    /// @brief The StringHash of the type being captured.
    const string_hash m_type_hash;
};


/**
 * @brief Begins capturing a new type. Returns a reflector to be used in a builder pattern.
 * @tparam T The type to capture.
 * @param ctx The context to capture into.
 * @param type_name The name to associate with T.
 * @return An instance of a reflector, used to sequentially capture a new type.
 */
template <typename T>
auto capture(context& ctx, const std::string_view type_name) -> reflector<T>
{
    return reflector<T>(ctx, alias<T>{ type_name }.hash);
}

/**
 * @brief Begins capturing a new type in a local context. Returns a reflector to be used in a builder pattern.
 * @tparam T The type to capture.
 * @param type_name The name to associate with T.
 * @return An instance of a reflector, used to sequentially capture a new type.
 */
template <typename T>
auto capture(const std::string_view type_name) -> reflector<T>
{
    return reflector<T>(internal::global::ctx, alias<T>{ type_name }.hash);
}

/**
 * @brief Looks up and returns the type_info associated with T.
 * @tparam T The type to lookup.
 * @return The type_info associated with T.
 */
template <typename T>
auto lookup() -> type_info
{
    return internal::global::ctx[alias<T>::hash];
}

/**
 * @brief Looks up and returns the type_info associated with T.
 * @tparam T The type to lookup.
 * @param ctx The context source.
 * @return The type_info associated with T from the context ctx.
 */
template <typename T>
auto lookup(const context& ctx) -> type_info
{
    return ctx[alias<T>::alias];
}

/**
 * @brief Looks up and returns the type_info associated with the name.
 * @param name The name to lookup.
 * @return The type_info associated with the name.
 */
inline auto lookup(const std::string_view name) -> const type_info&
{
    return internal::global::ctx.at(string_hash{ name });
}

/**
 * @brief Looks up and returns the type_info associated with the name.
 * @param ctx The context source.
 * @param name The name to lookup.
 * @return The type_info associated with the name.
 */
inline auto lookup(const context& ctx, const std::string_view name) -> const type_info&
{
    return ctx.at(string_hash{ name });
}

} // namespace reflex
