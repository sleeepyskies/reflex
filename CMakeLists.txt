cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

project(
        reflex
        DESCRIPTION "A simple reflection library written for c++20"
        HOMEPAGE_URL "https://github.com/sleeepyskies/reflex"
        LANGUAGES CXX
)

add_library(reflex INTERFACE)

target_include_directories(reflex INTERFACE src/)

