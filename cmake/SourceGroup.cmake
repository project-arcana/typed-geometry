
# properly groups sources for Qt Creator / Visual Studio
macro(arcana_source_group ARG_SOURCES)
    if (${CMAKE_GENERATOR} MATCHES "Visual Studio")
        source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${${ARG_SOURCES}})
    else()
        source_group("${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${${ARG_SOURCES}})
    endif()
endmacro()
