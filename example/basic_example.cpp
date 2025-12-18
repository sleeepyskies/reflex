#include "reflex.hpp"
#include <iostream>
#include <ranges>

struct component { };

struct pos_component : component
{
    float x, y, z;

    pos_component(const float x, const float y, const float z) : x(x), y(y), z(z) { }
};

struct tag_component : component
{
    std::string tag;
};

enum widget_type
{
    SLIDER,
    INPUT,
    SOMETHING
};

/// basic yaml output of a struct
int main(int argc, char* argv[])
{
    reflex::capture<pos_component>("pos_component")
            .field<&pos_component::x>("x")
                .decorate("min", 1.f)
                .decorate("max", 100.f)
                .decorate("widget", SLIDER)
            .field<&pos_component::y>("y")
                .decorate("min", 1.f)
                .decorate("max", 100.f)
                .decorate("widget", SLIDER)
            .field<&pos_component::z>("z")
                .decorate("min", 1.f)
                .decorate("max", 100.f)
                .decorate("widget", SLIDER);

    pos_component pos{ 1, 2, 3 };

    std::cout << "name: " << reflex::lookup("pos_component").name() << std::endl;

    for (const auto& field : reflex::lookup("pos_component").fields()) {
        std::cout << "\t" << field.name() << ": " << field.name() << std::endl;
        std::cout << "\t\t" << "min" << ": " << std::any_cast<float>(field.attribute("min")) << std::endl;
        std::cout << "\t\t" << "max" << ": " << std::any_cast<float>(field.attribute("max")) << std::endl;
    }
}
