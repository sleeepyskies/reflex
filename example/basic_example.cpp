#include "reflex.hpp"
#include <iostream>
#include <ranges>

struct rect
{
    struct position
    {
        int x, y;
    } pos;

    struct size
    {
        float x, y;
    } sz;

    static inline std::string foo = "foo";
    std::string type;
    const float area;
};

/// basic yaml output of a struct
int main(int argc, char* argv[])
{
    reflex::capture<rect>("rect")
            .field<&rect::pos>("pos")
            .field<&rect::type>("type")
            .field<&rect::sz>("sz")
            .field<&rect::area>("area");

    reflex::capture<rect::position>("rect::position")
            .field<&rect::position::x>("x")
            .field<&rect::position::y>("y");

    reflex::capture<rect::size>("rect::size")
            .field<&rect::size::x>("x")
            .field<&rect::size::y>("y");

    rect test{.pos = {.x = 4, .y = 5}, .sz = {.x = 10, .y = 10}, .type = "rectangle", .area = 100};

    std::cout << "---\n";
    std::cout << reflex::lookup("rect").name() << ":\n";
    for (const auto& field : reflex::lookup("rect").fields()) {
        std::cout << "\n";
    }
}
