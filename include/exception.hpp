/**
 * @file exception.hpp
 * @brief Reflection exception definitions.
*/
#pragma once

#include <stdexcept>
#include <string>


namespace reflex
{

/**
 * @brief Represents a general reflection error.
 */
class reflection_error final : public std::runtime_error
{
public:
    explicit reflection_error(const std::string& msg) : std::runtime_error("reflex: " + msg) { }

    reflection_error() : std::runtime_error("reflex: Reflection Error") { }
};

} // namespace reflex
