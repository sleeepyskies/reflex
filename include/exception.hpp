#pragma once

#include <exception>


namespace reflex
{

/**
 * @brief Represents a general reflection error.
 */
class reflection_error final : public std::runtime_error
{
public:
    explicit reflection_error(const std::string& msg) : std::runtime_error(msg) { }

    reflection_error() : std::runtime_error("Reflection Error") { }
};

} // namespace reflex
