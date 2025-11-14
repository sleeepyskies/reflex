#pragma once

#include <exception>


namespace reflex
{

/**
 * @brief Represents a general reflection error.
 */
class reflection_error : std::exception
{
public:
    reflection_error() = default;

    [[nodiscard]] auto what() const noexcept -> const char* override { return "Reflection error."; }
};

/**
 * @brief Represents an error when attempting to set a const object field.
 */
class const_field_error final : reflection_error
{
public:
    const_field_error() = default;

    [[nodiscard]] auto what() const noexcept -> const char* override { return "Cannot set a const field."; }
};

} // namespace reflex
