cmake_minimum_required(VERSION 3.16)
project(learn-tree-sitter
    LANGUAGES C CXX
    VERSION 0.1.0
    )

set(CMAKE_CXX_STANDARD 17)

# setup CPM.cmake
set(CPM_DOWNLOAD_VERSION 0.38.1)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_SOURCE_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
set(CPM_DOWNLOAD_URL "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake")
set(CPM_DOWNLOAD_VERIFY_CHECKSUM true)
set(CPM_DOWNLOAD_VERIFY_CHECKSUM_DIGEST "SHA256=9d2072c167bd4b08fb60087553c146515e3f5be061353c321a6d5496eb4bf9ea")

if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    file(DOWNLOAD ${CPM_DOWNLOAD_URL} ${CPM_DOWNLOAD_LOCATION}
        EXPECTED_HASH ${CPM_DOWNLOAD_VERIFY_CHECKSUM_DIGEST}
        )
endif()

include(${CPM_DOWNLOAD_LOCATION})

# add tree-sitter deps
CPMAddPackage(
    NAME tree-sitter
    GITHUB_REPOSITORY tree-sitter/tree-sitter
    GIT_TAG v0.20.7
    DOWNLOAD_ONLY YES
    )

add_custom_command(
    OUTPUT ${tree-sitter_SOURCE_DIR}/libtree-sitter.a
    COMMAND make libtree-sitter.a
    WORKING_DIRECTORY ${tree-sitter_SOURCE_DIR}
    COMMENT "Building libtree-sitter.a"
    )

add_custom_target(tree-sitter-build
    DEPENDS ${tree-sitter_SOURCE_DIR}/libtree-sitter.a
    )

add_library(tree-sitter STATIC IMPORTED GLOBAL)
set_target_properties(tree-sitter PROPERTIES
    IMPORTED_LOCATION ${tree-sitter_SOURCE_DIR}/libtree-sitter.a
    )

# learn_tree_sitter
add_executable(learn_tree_sitter
    src/test_parser.c
    src/parser.c
    )
add_dependencies(learn_tree_sitter tree-sitter-build)

target_include_directories(learn_tree_sitter PUBLIC
    ${CMAKE_SOURCE_DIR}/tree-sitter/include
    )

target_link_libraries(learn_tree_sitter
    PRIVATE tree-sitter
    )

# use_parser
add_executable(use_parser
    src/parser.c
    src/use_parser.cc
    )
add_dependencies(use_parser tree-sitter-build)

target_include_directories(use_parser PUBLIC
    ${CMAKE_SOURCE_DIR}/tree-sitter/include
    )

target_link_libraries(use_parser
    PRIVATE tree-sitter
    )
