
# properly groups sources for Qt Creator / Visual Studio
#
# usage:
#
# file(GLOB_RECURSE SOURCES "src/*.cc")
# file(GLOB_RECURSE HEADERS "src/*.hh" "src/*.inl")
# arcana_source_group(SOURCES HEADERS)
#
#
macro(arcana_source_group)
    foreach(loop_var ${ARGN})

        if (${CMAKE_GENERATOR} MATCHES "Visual Studio")
            source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${${loop_var}})
        else()
            source_group("${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${${loop_var}})
        endif()

    endforeach()
endmacro()
