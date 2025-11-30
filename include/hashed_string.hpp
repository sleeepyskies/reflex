#pragma once

#include <cstdint>


namespace reflex
{

/**
 * @brief A class representing a hashed string.
 */
class hashed_string
{
public:
    /**
     * @brief Constructs a new hashed_string given a const char*.
     * @param string The null terminated string to hash.
     */
    constexpr explicit hashed_string(const char* string) : m_hash(fnv1a(string)), m_name(string) { }

    /**
     * @brief Three-way comparison between two hashed_string's.
     */
    [[nodiscard]] constexpr auto operator<=>(const hashed_string& other) const noexcept
    {
        return m_hash <=> other.m_hash;
    }

    /**
     * @brief Returns the computed 64-bit hash.
     * @return The hash value.
     */
    [[nodiscard]] constexpr auto value() const noexcept -> uint64_t { return m_hash; }

    /**
     * @brief Returns the original string used to construct this hash.
     * @return The original string.
     */
    [[nodiscard]] constexpr auto data() const noexcept -> const char* { return m_name; }

private:
    /// @brief The computed hash.
    uint64_t m_hash;
    /// @brief The original string.
    const char* m_name;

    /**
     * @brief Hashes a null terminated string using the FNV-1a hash algorithm.
     * @see https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
     * @param str The null terminated string to hash.
     * @return A 64-bit hash of the input string.
     */
    static constexpr auto fnv1a(const char* str) -> uint64_t
    {
        // ReSharper disable once CppTooWideScope
        constexpr uint64_t prime  = 1099511628211ull;
        constexpr uint64_t offset = 14695981039346656037ull;
        uint64_t hash             = offset;
        const char* c             = str;
        while (*c) {
            hash ^= *c;
            hash *= prime;
            c++;
        }
        return hash;
    }
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
