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

if (UNIX)
    message("[${PROJECT_NAME}] Adding GMP dependency")
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(GMP REQUIRED IMPORTED_TARGET gmp)
    pkg_check_modules(GMPXX REQUIRED IMPORTED_TARGET gmpxx)

    target_link_libraries(tests-typed-geometry PUBLIC PkgConfig::GMPXX PkgConfig::GMP)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux" AND (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64)|(i686)|(i386)"))
    target_compile_options(tests-typed-geometry PUBLIC -msse2 -msse3 -mavx2 -mbmi -mbmi2 -mlzcnt)
endif()