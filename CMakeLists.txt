cmake_minimum_required(VERSION 3.8)
project(TypedGeometry)

# ===============================================
# Create target

file(GLOB_RECURSE SOURCES
    "src/*.cc"
    "src/*.hh"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_library(typed-geometry INTERFACE)

target_include_directories(typed-geometry INTERFACE "src")

add_custom_target(typed-geometry-src SOURCES ${SOURCES})
