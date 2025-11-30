#pragma once

#include <string_view>

#include "type_info.hpp"
#include "field.hpp"
#include "hashed_string.hpp"
#include "traits.hpp"
#include "context.hpp"


namespace reflex
{

/**
 * @brief Builds and registers type metadata. Used for constructing type_info.
 * @tparam T The reflected type.
 */
template <typename T>
class reflector
{
public:
    /**
     * @brief Creates a Reflector bound to a context.
     * @param ctx Reflection context.
     * @param hash The string_hash to associate with T
     */
    explicit reflector(context& ctx, const type_hash& hash) noexcept : m_ctx(&ctx), m_type_hash(hash)
    {
        m_ctx->try_emplace(m_type_hash, type_info{ m_type_hash });
    }

    /**
     * @brief Captures a member field entry.
     * @tparam Ptr The member pointer of the field.
     * @param field_name The name of the field.
     * @return A reference to the reflector for use in a builder pattern.
     */
    template <auto Ptr>
        requires member_field_ptr<Ptr>
    auto field(const std::string_view field_name) noexcept -> reflector&
    {
        auto& ctx = *m_ctx;
        const name_hash name_hash{ field_name };
        ctx.at(m_type_hash).insert_field(name_hash, field::create_member(name_hash, m_type_hash, Ptr));
        return *this;
    }

    /**
     * @brief Captures a static field entry.
     * @tparam Ptr The static pointer to the field.
     * @param field_name The name of the field.
     * @return A reference to the reflector for use in a builder pattern.
     */
    template <auto Ptr>
        requires static_field_ptr<Ptr>
    auto field(const std::string_view field_name) noexcept -> reflector&
    {
        auto& ctx = *m_ctx;
        const name_hash name_hash{ field_name };
        ctx.at(m_type_hash).insert_field(name_hash, field::create_static(name_hash, m_type_hash, Ptr));
        return *this;
    }

private:
    /// @brief The context to capture into.
    context* m_ctx;
    /// @brief The type_hash of the type being captured.
    const type_hash m_type_hash;
};


} // namespace reflex
