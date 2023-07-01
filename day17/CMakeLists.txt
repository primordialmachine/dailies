# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.day17")


# One list for inlays files, one list for source files, one list for headers, one list for readmes.
set(${name}.inlays "")
set(${name}.sources "")
set(${name}.headers "")
set(${name}.readmes "")

# "README.md".
set(${name}.readmes "README.md")

# "main.c".
list(APPEND ${name}.sources "main.c")

# "dx/scenes/*".
list(APPEND ${name}.sources "dx/scenes/mesh_viewer_scene.c")
list(APPEND ${name}.headers "dx/scenes/mesh_viewer_scene.h")
list(APPEND ${name}.sources "dx/scenes/create_assets.c")
list(APPEND ${name}.headers "dx/scenes/create_assets.h")

# The target architecture determines which implementation to compile.
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  message( STATUS "detected x64 target architecture" )
  set( infix "x64" )
elseif( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  message( STATUS "detected x86 target architecture" )
  set( infix "x86" )
else()
  message( FATAL_ERROR "unable to detect architecture" )
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.headers})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.inlays})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.asms})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.readmes})

project(${name})

add_executable(${name} ${${name}.sources} ${${name}.headers} ${${name}.readmes} ${${name}.inlays} ${${name}.asms})

# DLL plugins.
add_dependencies(${name} tif-writer)
add_custom_command(TARGET ${name} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${CMAKE_CURRENT_BINARY_DIR}/../tif-writer/$<CONFIG>/tif-writer.dll $<TARGET_FILE_DIR:${name}>
                   COMMAND_EXPAND_LISTS)

# See ${CMAKE_SOURCE_DIR_DIR}/CMakeLibrary.cmake for more information.
dx_configure_warnings(${name})

target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${name} dailies.engine)
target_link_libraries(${name} dailies.val.gl)

find_package(Doxygen)

if (DOXYGEN_FOUND)
  # Copy the doxygen configuration file to the out of source build directory.
  # Also adjust the copied configuration file.
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp
    DEPENDS ${doxyfile} ${${name}.sources} ${${name}.headers} ${${name}.readmes} ${${name}.inlays} ${${name}.asms}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND "doxygen" ${DOXYGEN_OUT}
    COMMAND cmake -E touch ${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp
    COMMENT "generating API documentation with Doxygen"
    VERBATIM)

  add_custom_target(${name}.documentation ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp)
endif (DOXYGEN_FOUND)

# Copy the assets to the current binary directory.
file(GLOB_RECURSE files_to_copy RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.*" )

foreach (file_to_copy ${files_to_copy})
  add_custom_command(
    TARGET ${name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/assets/${file_to_copy}"
                                                  "${CMAKE_CURRENT_BINARY_DIR}/assets/${file_to_copy}"
    COMMAND_EXPAND_LISTS
  )
endforeach()
