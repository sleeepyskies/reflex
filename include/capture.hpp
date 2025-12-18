#pragma once

#include "traits.hpp"
#include "alias.hpp"


namespace reflex
{
template <typename T>
class reflector
{
public:
    reflector(context* ctx, const hashed_string& hash) : m_ctx(ctx), m_type_hash(hash)
    {
        ctx->emplace(hash, internal::type_descriptor{ hash, sizeof(T), { } });
    }

    template <auto Ptr>
        requires field_ptr<Ptr>
    auto field(const char* field_name) noexcept -> reflector&
    {
        auto& ctx        = *m_ctx;
        using field_type = member_info<decltype(Ptr)>::field_type;
        using class_type = member_info<decltype(Ptr)>::class_type;

        // god
        const size_t offset = reinterpret_cast<size_t>(&(((class_type*)0)->*Ptr));

        internal::type_descriptor& desc = ctx.at(m_type_hash);
        desc.fields.emplace_back(hashed_string{ field_name }, internal::alias<field_type>::hash, offset, std::unordered_map<hashed_string, std::any>{ });

        return *this;
    }

    auto decorate(const char* key, std::any val) -> reflector&
    {
        // todo: kinda strange idk, maybe child struct instead with ref to parent? or pass decorate args direct to field()?
        auto& ctx                       = *m_ctx;
        internal::type_descriptor& desc = ctx.at(m_type_hash);
        desc.fields.back().attributes.emplace(hashed_string{ key }, val);
        return *this;
    }

private:
    context* m_ctx;
    const hashed_string m_type_hash;
};
} // namespace reflex
