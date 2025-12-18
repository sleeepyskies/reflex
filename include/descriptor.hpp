#pragma once

#include <vector>
#include <any>


namespace reflex::internal
{

struct field_descriptor
{
    hashed_string field_hash;
    hashed_string type_hash;
    size_t offset;
    // todo: do we acc need a map here?
    std::unordered_map<hashed_string, std::any> attributes; //< Additional user defined meta data, useful for GUI's.
};

struct type_descriptor
{
    hashed_string hash;
    size_t size;
    std::vector<field_descriptor> fields{ };
    // std::vector<hashed_string> funcs{ };
    // std::vector<hashed_string> bases{ };
};

}
