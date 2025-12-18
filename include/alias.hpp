#pragma once

#include "hashed_string.hpp"


namespace reflex::internal
{
template <typename T>
struct alias
{
    explicit alias(const char* str) { if (!hash.data()) hash = hashed_string{ str }; }

    /// @brief A per-type cached hash value. Constructing an alias with a
    /// string will initialize the static hash for that type so later lookups
    /// can reference it without supplying the name again.
    static inline hashed_string hash;
};
} // namespace internal
