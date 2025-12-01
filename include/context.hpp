#pragma once

#include <unordered_map>
#include "hashed_string.hpp"
#include "type_info.hpp"


namespace reflex
{

/// @brief A Storage container for reflected types.
using context = std::unordered_map<hashed_string, type_info>;


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


} // namespace reflex