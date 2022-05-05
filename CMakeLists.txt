cmake_minimum_required(VERSION 3.16)
project(learn-tree-sitter
    LANGUAGES C CXX
    VERSION 0.1.0
    )

set(CMAKE_CXX_STANDARD 17)

add_executable(learn_tree_sitter
    src/test_parser.c
    src/parser.c
    )

target_include_directories(learn_tree_sitter PUBLIC
    ${CMAKE_SOURCE_DIR}/tree-sitter/include
    )

target_link_libraries(learn_tree_sitter
    ${CMAKE_SOURCE_DIR}/src/libtree-sitter.a
    )