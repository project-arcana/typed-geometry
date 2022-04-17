cmake_minimum_required(VERSION 3.11)

file(GLOB_RECURSE SOURCES
    "*.cc"
    "*.hh"
)

add_arcana_test(tests-typed-geometry "${SOURCES}")

target_include_directories(tests-typed-geometry PUBLIC ./)

target_link_libraries(tests-typed-geometry PUBLIC
    clean-core
    clean-ranges
    typed-geometry
    rich-log
    ctracer
    $<$<PLATFORM_ID:Linux>:gmp>
)

target_compile_options(tests-typed-geometry PUBLIC -mavx2 -mbmi2 -mlzcnt)
