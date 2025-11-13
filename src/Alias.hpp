#pragma once
#include "StringHash.hpp"


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
    constexpr explicit Alias(const std::string_view a) { hash = StringHash{ a }; }

    static inline StringHash hash;
};

} // namespace reflex
