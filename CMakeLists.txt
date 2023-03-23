cmake_minimum_required(VERSION 3.8)
project(TypedGeometry)

if (NOT TARGET clean-core)
    message(FATAL_ERROR "[typed-geometry] clean-core must be available")
endif()

include(cmake/IsCPUExtensionSupported.cmake)
is_cpu_extension_supported(BMI2_SUPPORTED "bmi2")
is_cpu_extension_supported(LZCNT_SUPPORTED "lzcnt")

option(TG_EXPORT_LITERALS "if true, spills tg::literals into the global namespace (i.e. 180_deg works out of the box)" ON)
option(TG_EIGEN_TESTS "Build typed-geometry tests that require eigen" OFF)
option(TG_IMPLEMENTATION_REPORT "Test-case that generates implementation coverage" OFF)

if(BMI2_SUPPORTED AND LZCNT_SUPPORTED)
    option(TG_ENABLE_FIXED_INT "if true, enables TGs fixed_int feature. Requires a modern CPU" ON)
else()
    message("fixed_int feature disabled as it requires support for bmi2 and lzcnt!")
    option(TG_ENABLE_FIXED_INT "if true, enables TGs fixed_int feature. Requires a modern CPU" OFF)
endif()

# ===============================================
# Create target

file(GLOB_RECURSE SOURCES
    "src/*.cc"
    "src/*.hh"
)

if(NOT TG_ENABLE_FIXED_INT)
    list(FILTER SOURCES EXCLUDE REGEX ".*fixed_int.*")
    list(FILTER SOURCES EXCLUDE REGEX ".*fixed_uint.*")
endif()



# =========================================
# define library

file(GLOB_RECURSE SOURCES "src/*.cc")
file(GLOB_RECURSE HEADERS "src/*.hh")

arcana_add_library(TG typed-geometry SOURCES HEADERS)

target_include_directories(typed-geometry PUBLIC src/)

target_link_libraries(typed-geometry PUBLIC
    clean-core
)

if (TG_EXPORT_LITERALS)
    target_compile_definitions(typed-geometry PUBLIC TG_EXPORT_LITERALS)
endif()

if(TG_IMPLEMENTATION_REPORT)
    target_compile_definitions(typed-geometry PUBLIC TG_IMPLEMENTATION_REPORT)
endif()
