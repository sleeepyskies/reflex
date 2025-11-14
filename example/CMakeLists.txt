add_executable(reflex_example basic_example.cpp)

# Include reflex headers
target_include_directories(reflex_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Link header-only reflex library
target_link_libraries(reflex_example PRIVATE reflex)
