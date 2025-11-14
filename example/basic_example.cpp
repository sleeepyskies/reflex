#include "reflex.hpp"
#include <iostream>
#include <ranges>

struct [[reflex]] rect
{
    static inline std::string foo = "foo";
    std::string type;
    int w, h;
    const float area;
};

std::any test()
{
    return 4;
}

/// basic yaml output of a struct
int main(int argc, char* argv[])
{
    reflex::capture<rect>("rect")
            .field<&rect::type>("square")
            .field<&rect::w>("w")
            .field<&rect::h>("h")
            .field<&rect::area>("area");

    rect pos{ .type = "square", .w = 3, .h = 3, .area = 9 };

    std::cout << "---\n";
    std::cout << reflex::lookup("rect").name() << ":\n";
    for (const auto& field : reflex::lookup("rect").fields()) {
        if (field.is_static()) { continue; }
        std::cout << "\t" << field.name() << ": ";
        if (field.hash() == reflex::alias<int>::hash) {
            std::cout << field.get<int>(pos);
        } else if (field.hash() == reflex::alias<float>::hash) {
            std::cout << field.get<float>(pos);
        } else if (field.hash() == reflex::alias<std::string>::hash) {
            std::cout << field.get<std::string>(pos);
        }
        std::cout << "\n";
    }
}
