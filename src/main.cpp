#include <iostream>

#include "StringHash.hpp"
#include "reflex.hpp"

struct Person
{
    int age;
    int salary;
    static inline int averageLifespan = 80;
};

struct Thing
{
    int foo;
};

int main() {
    reflex::capture<Person>("Person")
        .field<&Person::age>("age")
        .field<&Person::salary>("salary")
        .field<&Person::averageLifespan>("averageLifespan");

    const Person p{25, 50000};

    auto typeData = reflex::lookup<Person>();

    std::cout << "{\n";
    for (auto& [name, field] : typeData.fields) {
        std::cout << "  \"" << std::string(name.getString()) << "\": ";
        if (field.isStatic) {
            const int* ptr = static_cast<const int*>(field.get(nullptr));
            std::cout << *ptr;
        } else {
            const int* ptr = static_cast<const int*>(field.get((void*)&p));
            std::cout << *ptr;
        }
        std::cout << ",\n";
    }
    std::cout << "}\n";

    return 0;
}

