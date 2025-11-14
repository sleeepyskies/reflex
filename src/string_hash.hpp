#pragma once

#include <cstdint>
#include <string_view>


namespace reflex
{

/**
 * @brief A class representing a hashed string.
 */
class string_hash final
{
public:
    /**
     * @brief Constructs a new string_hash given a std::string_view.
     * @param string The string to hash.
     */
    constexpr explicit string_hash(const std::string_view string) : m_hash(fnv1a(string)), m_name(string) { }

    /**
     * @brief Constructs a default string_hash.
     */
    constexpr string_hash() : m_hash(0) { }

    /**
     * @brief Compares two string_hash items for equality.
     * @param other The other string_hash to compare.
     * @return true if both hashes are equal, false otherwise.
     */
    constexpr bool operator==(const string_hash& other) const noexcept { return m_hash == other.m_hash; }

    /**
     * @brief Compares two string_hash items for inequality.
     * @param other The other string_hash to compare.
     * @return true if both hashes are inequal, false otherwise.
     */
    constexpr bool operator!=(const string_hash& other) const noexcept { return m_hash != other.m_hash; }

    /**
     * @brief Compares if this hash is LE to another hash.
     * @param other The other string_hash to compare.
     * @return true if this hash is LE other.hash.
     */
    constexpr bool operator<=(const string_hash& other) const noexcept { return m_hash <= other.m_hash; }

    /**
     * @brief Compares if this hash is GE to another hash.
     * @param other The other string_hash to compare.
     * @return true if this hash is GE other.hash.
     */
    constexpr bool operator>=(const string_hash& other) const noexcept { return m_hash >= other.m_hash; }

    /**
     * @brief Compares if this hash is GT to another hash.
     * @param other The other string_hash to compare.
     * @return true if this hash is GT other.hash.
     */
    constexpr bool operator<(const string_hash& other) const noexcept { return m_hash < other.m_hash; }

    /**
     * @brief Compares if this hash is LT to another hash.
     * @param other The other string_hash to compare.
     * @return true if this hash is LT other.hash.
     */
    constexpr bool operator>(const string_hash& other) const noexcept { return m_hash > other.m_hash; }

    /**
     * @brief Returns the computed 64-bit hash.
     * @return The hash value.
     */
    [[nodiscard]] auto hash() const noexcept -> uint64_t { return m_hash; }

    /**
     * @brief Returns the original string used to construct this hash.
     * @return The original string.
     */
    [[nodiscard]] auto name() const noexcept -> std::string_view { return m_name; }

private:
    /// @brief The computed hash.
    uint64_t m_hash;
    /// @brief The original string.
    std::string_view m_name;

    /**
     * @brief Hashes a null terminated string using the FNV-1a hash algorithm.
     * @see https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
     * @param str The null terminated string to hash.
     * @return A 64-bit hash of the input string.
     */
    static constexpr auto fnv1a(const std::string_view str) -> uint64_t
    {
        constexpr uint64_t offset = 14695981039346656037ull;
        // ReSharper disable once CppTooWideScope
        constexpr uint64_t prime  = 1099511628211ull;
        uint64_t hash             = offset;
        for (const auto c : str) {
            hash ^= c;
            hash *= prime;
        }
        return hash;
    }
};

} // namespace reflex

// todo: should create my own hashmap implementation, which would avoid needing this

template <>
struct std::hash<reflex::string_hash>
{
    size_t operator()(const reflex::string_hash& s) const noexcept { return s.hash(); }
};
