/**
 * @file reflex.hpp
 * @brief Single include header to ease of use.
 */
#pragma once

#include "context.hpp"
#include "exception.hpp"
#include "field.hpp"
#include "hash.hpp"
#include "reflector.hpp"
#include "serialize.hpp"
#include "traits.hpp"
#include "type_info.hpp"


namespace reflex
{

namespace internal
{

template <typename T>
struct alias
{
    explicit alias(const std::string_view str) { hash = type_hash{ str }; }

    /// @brief A per-type cached hash value. Constructing an alias with a
    /// string will initialize the static hash for that type so later lookups
    /// can reference it without supplying the name again.
    static inline type_hash hash;
};

} // namespace internal


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
    return reflector<T>(ctx, internal::alias<T>{ type_name }.hash);
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
    return reflector<T>(internal::global::ctx, internal::alias<T>{ type_name }.hash);
}

/**
 * @brief Looks up and returns the type_info associated with T.
 * @tparam T The type to lookup.
 * @return The type_info associated with T.
 */
template <typename T>
auto lookup() -> type_info
{
    return internal::global::ctx[internal::alias<T>::hash];
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
    return ctx[internal::alias<T>::hash];
}

/**
 * @brief Looks up and returns the type_info associated with the name.
 * @param name The name to lookup.
 * @return The type_info associated with the name.
 */
inline auto lookup(const std::string_view name) -> const type_info&
{
    return internal::global::ctx.at(type_hash{ name });
}

/**
 * @brief Looks up and returns the type_info associated with the name.
 * @param ctx The context source.
 * @param name The name to lookup.
 * @return The type_info associated with the name.
 */
inline auto lookup(const context& ctx, const std::string_view name) -> const type_info&
{
    return ctx.at(type_hash{ name });
}

} // namespace reflex
