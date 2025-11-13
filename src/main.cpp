#include <iostream>

#include "StringHash.hpp"
#include "reflex.hpp"

struct [[reflex]] [[serial]] Person
{
    int age;
    int salary;
    std::string name;
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
        .field<&Person::name>("name")
        .field<&Person::averageLifespan>("averageLifespan");

    const Person p1{25, 50000, "bill"};
    Person p2{25, 50000, "bill"};

    const auto field = reflex::lookup("Person").lookup_field("age");
    auto& val1 = *static_cast<const int*>(field.get(p1));
    auto& val2 = *static_cast<int*>(field.get(p2));
    field.set(p2, "hfjk");

    return 0;
}

