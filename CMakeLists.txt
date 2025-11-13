cmake_minimum_required(VERSION 4.0)
project(reflex)

set(CMAKE_CXX_STANDARD 20)

add_executable(reflex
        src/main.cpp
        src/reflex.hpp
        src/StringHash.hpp
        src/Context.hpp
        src/traits.hpp
)
