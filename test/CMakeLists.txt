add_executable(reflex_test reflex.cpp)

target_include_directories(reflex_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(reflex_test PRIVATE reflex)

add_test(NAME reflex_test COMMAND reflex_test)
