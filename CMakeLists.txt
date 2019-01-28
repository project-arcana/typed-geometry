cmake_minimum_required(VERSION 3.8)
project(TypedGeometry)

# ===============================================
# Create target

file(GLOB_RECURSE SOURCES
    "src/*.cc"
    "src/*.hh"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_library(typed-geometry STATIC ${SOURCES})

target_include_directories(typed-geometry PUBLIC "src")

if (CMAKE_CXX_STANDARD GREATER_EQUAL 17)
    target_compile_definitions(typed-geometry PUBLIC TG_SUPPORT_CXX17)
endif()
