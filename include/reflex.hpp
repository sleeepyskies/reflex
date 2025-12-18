/**
 * @file reflex.hpp
 * @brief Single include header to ease of use, also holds the main user API.
 */
#pragma once

#include "context.hpp"
#include "exception.hpp"
#include "hashed_string.hpp"
#include "handle.hpp"
#include "range.hpp"
#include "alias.hpp"
#include "capture.hpp"


namespace reflex
{


/**
 * @brief Begins capturing a new type. Returns a reflector to be used in a builder pattern.
 * @tparam T The type to capture.
 * @param ctx The context to capture into.
 * @param type_name The name to associate with T.
 * @return An instance of a reflector, used to sequentially capture a new type.
 */
template <typename T>
auto capture(context& ctx, const char* type_name) noexcept -> reflector<T>
{
    return reflector<T>(&ctx, internal::alias<T>{ type_name }.hash);
}

/**
 * @brief Begins capturing a new type in a local context. Returns a reflector to be used in a builder pattern.
 * @tparam T The type to capture.
 * @param type_name The name to associate with T.
 * @return An instance of a reflector, used to sequentially capture a new type.
 */
template <typename T>
auto capture(const char* type_name) noexcept -> reflector<T>
{
    return reflector<T>(&internal::global::ctx, internal::alias<T>{ type_name }.hash);
}

/**
 * @brief Looks up and returns the type_handle associated with T.
 * @tparam T The type to lookup.
 * @throws reflection_error if the type T has not been captured.
 * @return The type_info associated with T.
 */
template <typename T>
auto lookup() -> type_handle
{
    auto hash = internal::alias<T>::hash;
    if (!hash) {
        throw reflection_error{ "Hash does not exist for this type." };
    }
    auto& ctx = internal::global::ctx;
    auto it   = ctx.find(internal::alias<T>::hash);
    if (it == ctx.end()) {
        throw reflection_error{ "Attempted to lookup type that has not been captured." };
    }
    return type_handle{ &ctx, &it->second };
}

/**
 * @brief Looks up and returns the type_handle associated with T.
 * @tparam T The type to lookup.
 * @param ctx The context source.
 * @throws reflection_error if the type T has not been captured.
 * @return The type_info associated with T from the context ctx.
 */
template <typename T>
auto lookup(const context& ctx) -> type_handle
{
    auto hash = internal::alias<T>::hash;
    if (!hash) {
        throw reflection_error{ "Hash does not exist for this type." };
    }
    auto it = ctx.find(hash);
    if (it == ctx.end()) {
        throw reflection_error{ "Attempted to lookup type that has not been captured." };
    }
    return type_handle{ &ctx, &it->second };
}

/**
 * @brief Looks up and returns the type_info associated with the name.
 * @param name The name to lookup.
 * @throws reflection_error if the type has not been captured.
 * @return The type_info associated with the name.
 */
inline auto lookup(const char* name) -> type_handle
{
    auto& ctx     = internal::global::ctx;
    const auto it = ctx.find(hashed_string{ name });
    if (it == ctx.end()) {
        throw reflection_error{ "Attempted to lookup type that has not been captured." };
    }
    return type_handle{ &ctx, &it->second };
}

/**
 * @brief Looks up and returns the type_info associated with the name.
 * @param ctx The context source.
 * @param name The name to lookup.
 * @throws reflection_error if the type has not been captured.
 * @return The type_info associated with the name.
 */
inline auto lookup(context& ctx, const char* name) -> type_handle
{
    const auto it = ctx.find(hashed_string{ name });
    if (it == ctx.end()) {
        throw reflection_error{ "Attempted to lookup type that has not been captured." };
    }
    return type_handle{ &ctx, &it->second };
}

/**
 * @brief Returns the captured name of the type T.
 * @tparam T The type to get the name for.
 * @throws reflection_error if the type has not been captured.
 * @return The name of the type.
 */
template <typename T>
auto name() -> const char* { return lookup<T>().name(); }

} // namespace reflex
