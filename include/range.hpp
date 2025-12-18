#pragma once

#include "context.hpp"

namespace reflex
{

template <typename Handle, typename Descriptor>
class iterator
{
public:
    iterator(context* ctx, internal::field_descriptor* data) : m_ctx(ctx), m_data(data) { }

    auto operator++() -> iterator& { ++m_data; return *this; }
    auto operator++(int) -> iterator { const auto it = *this; ++*this; return it; }
    auto operator--() -> iterator& { --m_data; return *this; }
    auto operator--(int) -> iterator { const auto it = *this; --*this; return it; }

    auto operator[](const size_t index) const -> Handle { return Handle{m_ctx, m_data + index}; }
    auto operator->() const -> Handle { return operator[](0); }
    auto operator*() const -> Handle { return operator[](0); }

    auto operator==(const iterator& other) const -> bool { return m_data == other.m_data; }
    auto operator!=(const iterator& other) const -> bool { return m_data != other.m_data; }

private:
    context* m_ctx;
    Descriptor* m_data;
};

template <typename Handle, typename Descriptor>
class range
{
public:
    using iterator = iterator<Handle, Descriptor>;

    range(context* ctx, internal::field_descriptor* data, const size_t size) : m_ctx(ctx), m_data(data),
        m_size(size) { }

    auto begin() const -> iterator { return iterator{ m_ctx, m_data }; }
    auto end() const -> iterator { return iterator{ m_ctx, m_data + m_size }; }

private:
    context* m_ctx;
    Descriptor* m_data;
    size_t m_size;
};

}
