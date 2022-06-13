cmake_minimum_required(VERSION 3.11)

file(GLOB_RECURSE SOURCES
    "*.cc"
    "*.hh"
)

add_arcana_test(tests-typed-geometry "${SOURCES}")

target_include_directories(tests-typed-geometry PUBLIC ./)

target_link_libraries(tests-typed-geometry PUBLIC
    clean-core
    typed-geometry
    rich-log
    ctracer
    $<$<PLATFORM_ID:Linux>:gmp>
)

if (NOT MSVC)
    target_compile_options(tests-typed-geometry PUBLIC -msse2 -msse3 -mavx2 -mbmi -mbmi2 -mlzcnt)
endif()
