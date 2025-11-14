#pragma once

#include "string_hash.hpp"


namespace reflex
{

/**
 * @brief Helper struct to create an alias between a StringHash and a type T.
 * @tparam T The type to create an alias for.
 * @remark Constructing a second Alias<T> will overwrite the previous alias.
 */
template <typename T>
struct alias
{
    /**
     * @brief Associates a type T with a StringHash.
     * @param a The string to associate T with. Will be used to construct a hash.
     */
    constexpr explicit alias(const std::string_view a) { hash = string_hash{ a }; }

    /// @brief The StringHash associated with T.
    static inline string_hash hash;
};

} // namespace reflex
