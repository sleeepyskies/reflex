#pragma once

#include <type_traits>


namespace reflex
{

/// @brief A member field pointer.
template <auto Ptr>
concept MemberFieldPointer = std::is_member_object_pointer_v<decltype(Ptr)>;

/// @brief A static field pointer.
template <auto Ptr>
concept StaticFieldPointer =
        std::is_pointer_v<decltype(Ptr)> &&
        !std::is_member_object_pointer_v<decltype(Ptr)> &&
        !std::is_member_function_pointer_v<decltype(Ptr)> &&
        !std::is_function_v<std::remove_pointer_t<decltype(Ptr)>>;

/// @brief A general field pointer. Maybe be static field or a member field.
template <auto Ptr>
concept FieldPointer = MemberFieldPointer<Ptr> || StaticFieldPointer<Ptr>;


} // namespace reflex
