cmake_minimum_required(VERSION 3.8)
project(TypedMath)

# ===============================================
# Create target

file(GLOB_RECURSE SOURCES
    "src/*.cc"
    "src/*.hh"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_library(typed-math INTERFACE)

target_include_directories(typed-math INTERFACE "src")
