#pragma once

#include <cstdint>


namespace reflex
{
namespace internal
{
// Magic numbers used for 64-bit FNV-1a hashing.

constexpr auto prime  = 1099511628211ull;
constexpr auto offset = 14695981039346656037ull;
}


/**
 * @brief A class representing a hashed string.
 */
class hashed_string
{
public:
    /**
     * @brief Creates a new hashes_string using the FNV-1a hash algorithm.
     * @see https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
     * @param string The null terminated string to hash.
     * @return A 64-bit hash of the input string.
     */
    constexpr explicit hashed_string(const char* string) : m_hash(internal::offset), m_name(string), m_length(0)
    {
        for (const char* c = string; *c; c++) {
            m_hash ^= *c;
            m_hash *= internal::prime;
            m_length++;
        }
    }

    constexpr explicit hashed_string() : m_hash(0), m_name(nullptr), m_length(0) { }

    /**
     * @brief Three-way comparison between two hashed_string's.
     */
    [[nodiscard]] constexpr auto operator<=>(const hashed_string& other) const noexcept
    {
        return m_hash <=> other.m_hash;
    }

    /**
     * @brief Equality comparison.
     */
    [[nodiscard]] constexpr auto operator==(const hashed_string& other) const noexcept -> bool
    {
        return m_hash == other.m_hash;
    }

    /**
     * @brief Inequality comparison.
     */
    [[nodiscard]] constexpr auto operator!=(const hashed_string& other) const noexcept -> bool
    {
        return !(*this == other);
    }

    /**
     * @brief Returns the computed 64-bit hash.
     */
    [[nodiscard]] constexpr auto value() const noexcept -> uint64_t { return m_hash; }

    /**
     * @brief Returns the original string used to construct this hash.
     */
    [[nodiscard]] constexpr auto data() const noexcept -> const char* { return m_name; }

    /**
     * @brief Returns the length of the original string.
     */
    [[nodiscard]] constexpr auto length() const noexcept -> size_t { return m_length; }

    [[nodiscard]] explicit operator bool() const noexcept { return m_hash != 0; }

private:
    /// @brief The computed hash.
    uint64_t m_hash;
    /// @brief The original string.
    const char* m_name;
    /// @brief The length of the original string.
    size_t m_length;
};
} // namespace reflex

/**
 * @brief Specialization of std::hash for reflex::hashed_string.
 *
 * Allows hashed_string to be used as a key in std::unordered_map or
 * std::unordered_set.
 */
template <>
struct std::hash<reflex::hashed_string>
{
    size_t operator()(const reflex::hashed_string& s) const noexcept
    {
        return s.value();
    }
};
