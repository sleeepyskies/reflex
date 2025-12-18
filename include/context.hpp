#pragma once

#include <memory>
#include <unordered_map>
#include "hashed_string.hpp"
#include "descriptor.hpp"

// todo: capture primitives? otherwise reflector wont work

namespace reflex
{
// todo: user ptrs here? in case of realloc
/// @brief A Storage container for reflected types.
using context = std::unordered_map<hashed_string, internal::type_descriptor>;


namespace internal
{
/**
 * @brief Global registration
 */
struct global
{
    /// @brief The global reflex context.
    static inline context ctx;
};
}
} // namespace reflex
