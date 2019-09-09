cmake_minimum_required(VERSION 3.8)
project(TypedGeometry)

option(TG_EXPORT_LITERALS "if true, spills tg::literals into the global namespace (i.e. 180_deg works out of the box)" ON)
option(TG_ENABLE_ASSERTIONS "if true, enables assertions (also in RelWithDebInfo)" ON)
option(TG_ENABLE_INTERNAL_ASSERTIONS "if true, enables internally used assertions (requires TG_ENABLE_ASSERTIONS)" ON)
option(TG_ENABLE_CONTRACTS "if true, enables contract assertions (requires TG_ENABLE_ASSERTIONS)" ON)

# ===============================================
# Create target

file(GLOB_RECURSE SOURCES
    "src/*.cc"
    "src/*.hh"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_library(typed-geometry STATIC ${SOURCES})
set_property(TARGET typed-geometry PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(typed-geometry PUBLIC "src")

target_compile_definitions(typed-geometry PUBLIC $<$<CONFIG:DEBUG>:TG_DEBUG>)
target_compile_definitions(typed-geometry PUBLIC $<$<CONFIG:RELEASE>:TG_RELEASE>)
target_compile_definitions(typed-geometry PUBLIC $<$<CONFIG:RELWITHDEBINFO>:TG_RELWITHDEBINFO>)

if (TG_ENABLE_ASSERTIONS)
    target_compile_definitions(typed-geometry PUBLIC $<$<CONFIG:DEBUG>:TG_ENABLE_ASSERTIONS>)
    target_compile_definitions(typed-geometry PUBLIC $<$<CONFIG:RELWITHDEBINFO>:TG_ENABLE_ASSERTIONS>)
endif()

if (TG_ENABLE_INTERNAL_ASSERTIONS)
    target_compile_definitions(typed-geometry PUBLIC TG_ENABLE_INTERNAL_ASSERTIONS)
endif()

if (TG_ENABLE_CONTRACTS)
    target_compile_definitions(typed-geometry PUBLIC TG_ENABLE_CONTRACTS)
endif()

if (TG_EXPORT_LITERALS)
    target_compile_definitions(typed-geometry PUBLIC TG_EXPORT_LITERALS)
endif()
