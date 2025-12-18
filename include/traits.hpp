#pragma once

#include <type_traits>


namespace reflex
{

/// @brief A member field pointer.
template <auto Ptr>
concept member_field_ptr = std::is_member_object_pointer_v<decltype(Ptr)>;

/// @brief A static field pointer.
template <auto Ptr>
concept static_field_ptr =
        std::is_pointer_v<decltype(Ptr)> &&
        !std::is_member_object_pointer_v<decltype(Ptr)> &&
        !std::is_member_function_pointer_v<decltype(Ptr)> &&
        !std::is_function_v<std::remove_pointer_t<decltype(Ptr)>>;

/// @brief A general field pointer. It may be a static field or a member field.
template <auto Ptr>
concept field_ptr = member_field_ptr<Ptr> || static_field_ptr<Ptr>;

/// @brief Helper type alias template to strip volatile/const/&/* from types.
template <typename T>
using stripped_type = std::remove_cvref_t<std::remove_pointer_t<T>>;

template <typename T>
struct member_info;

template <typename C, typename T>
struct member_info<T C::*>
{
    using field_type = T;
    using class_type = C;
};

} // namespace reflex
