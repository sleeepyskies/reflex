#pragma once

#include <exception>


namespace reflex
{

/**
 * @brief Represents a general reflection error.
 */
class reflection_error : public std::exception
{
public:
    explicit reflection_error(const char* message = "Reflection error.") noexcept : msg(message) { }

    [[nodiscard]] auto what() const noexcept -> const char* override { return msg; }

private:
    const char* msg;
};

/**
 * @brief Represents an error when attempting to set a const object field.
 */
class const_field_error final : reflection_error
{
public:
    const_field_error() : reflection_error("Cannot set a const field.") { }
};

} // namespace reflex
