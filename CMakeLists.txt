cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)

project(
        reflex
        DESCRIPTION "A simple reflection library written for c++20"
        HOMEPAGE_URL "https://github.com/sleeepyskies/reflex"
        LANGUAGES CXX
)
include(GNUInstallDirs)

add_library(reflex INTERFACE)

target_include_directories(
        reflex
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

option(REFLEX_INCLUDE_HEADERS "Include header files to target" ON)

# make IDE friendly (except clion urgh)
if (REFLEX_INCLUDE_HEADERS)
    message(STATUS "Including header files in the target")
    set(
            REFLEX_HEADERS

            include/reflex.hpp

            include/context.hpp
            include/exception.hpp
            include/field.hpp
            include/hashed_string.hpp
            include/reflector.hpp
            include/serialize.hpp
            include/traits.hpp
            include/type_info.hpp
    )

    target_sources(reflex INTERFACE ${REFLEX_HEADERS})
endif ()

# todo: write tests
# tests
option(REFLEX_ENABLE_TESTS "Enable tests when building" ON)
if (REFLEX_ENABLE_TESTS)
    enable_testing()
    message(STATUS "Running reflex tests")
    add_subdirectory(test)
endif ()

# examples
option(REFLEX_BUILD_EXAMPLES "Build examples" ON)
if (REFLEX_BUILD_EXAMPLES)
    message(STATUS "Building examples")
    add_subdirectory(example)
endif ()

