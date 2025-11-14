#pragma once

#include "string_hash.hpp"


namespace reflex
{

/**
 * @brief Helper struct to create an alias between a string_hash and a type T.
 * @tparam T The type to create an alias for.
 * @remark Constructing a second alias<T> will overwrite the previous alias.
 */
template <typename T>
struct alias
{
    /**
     * @brief Associates a type T with a string_hash.
     * @param name The string to associate T with. Will be used to construct a hash.
     */
    constexpr explicit alias(const std::string_view name) { hash = string_hash{ name }; }

    /// @brief The StringHash associated with T.
    static inline string_hash hash;
};

} // namespace reflex
