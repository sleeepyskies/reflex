#include "reflex.hpp"
#include <iostream>
#include <ranges>

static reflex::context s_component_ctx;

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

/// basic yaml output of a struct
int main(int argc, char* argv[])
{
    reflex::capture<pos_component>(s_component_ctx, "pos_component")
            .base("component")
            .field<&pos_component::x>("x")
            .field<&pos_component::y>("y")
            .field<&pos_component::z>("z");

    pos_component pos{ 1, 2, 3 };

    std::cout << "hash: " << reflex::lookup(s_component_ctx, "pos_component").get_hash().value() << std::endl;
    std::cout << "name: " << reflex::lookup(s_component_ctx, "pos_component").name() << std::endl;

    for (const auto& field : reflex::lookup(s_component_ctx, "pos_component").fields()) {
        std::cout << "\t" << field.name() << ": " << field.get(pos) << std::endl;
    }


    for (const auto& field_info : s_component_ctx | std::views::values) {
        if (scene.has<T>(entity)) {
            // draw divider, write title of component to gui etc
            for (const auto& field: field_info.fields()) {
                // somehow draw sliders/inputs for each field, also with given bounds
            }
        }
    }
}
