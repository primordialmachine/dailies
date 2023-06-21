# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.day15_to_day19")


set(${name}.inlays "")
set(${name}.sources "")
set(${name}.headers "")
set(${name}.readmes "")

# "README.md"
list(APPEND ${name}.readmes "README.md")

# "dx/application.(c|h)"
list(APPEND ${name}.sources "dx/application.c")
list(APPEND ${name}.headers "dx/application.h")

# "val" folder.
list(APPEND ${name}.sources "dx/val/texture.c" "dx/val/material.c" "dx/val/mesh.c" "dx/val/mesh_instance.c")
list(APPEND ${name}.headers "dx/val/texture.h" "dx/val/material.h" "dx/val/mesh.h" "dx/val/mesh_instance.h")
list(APPEND ${name}.sources "dx/val/program_text.c" "dx/val/command.c" "dx/val/program.c" "dx/val/context.c" "dx/val/cbinding.c" "dx/val/vbinding.c" "dx/val/buffer.c")
list(APPEND ${name}.headers "dx/val/program_text.h" "dx/val/command.h" "dx/val/program.h" "dx/val/context.h" "dx/val/cbinding.h" "dx/val/vbinding.h" "dx/val/buffer.h")

# "asset" folder.
list(APPEND ${name}.sources "dx/asset/mesh.c" "dx/asset/mesh_instance.c" "dx/asset/mesh/generators.c")
list(APPEND ${name}.headers "dx/asset/mesh.h" "dx/asset/mesh_instance.h" "dx/asset/mesh/generators.h")
list(APPEND ${name}.sources "dx/asset/mesh_operation.c")
list(APPEND ${name}.headers "dx/asset/mesh_operation.h")
list(APPEND ${name}.sources "dx/asset/mesh_operations/set_vertex_colors.c")
list(APPEND ${name}.headers "dx/asset/mesh_operations/set_vertex_colors.h")

list(APPEND ${name}.sources "dx/asset/scene.c")
list(APPEND ${name}.headers "dx/asset/scene.h")

list(APPEND ${name}.sources "dx/asset/texture.c" "dx/asset/material.c")
list(APPEND ${name}.headers "dx/asset/texture.h" "dx/asset/material.h")

list(APPEND ${name}.sources "dx/asset/image.c" "dx/asset/palette.c" "dx/asset/brushes.c" "dx/asset/color.c")
list(APPEND ${name}.headers "dx/asset/image.h" "dx/asset/palette.h" "dx/asset/brushes.h" "dx/asset/color.h")

# "adl" folder.
list(APPEND ${name}.sources "dx/adl/syntactical.c")
list(APPEND ${name}.headers "dx/adl/syntactical.h")

list(APPEND ${name}.sources "dx/adl/diagnostics.c")
list(APPEND ${name}.headers "dx/adl/diagnostics.h")

list(APPEND ${name}.sources "dx/adl/semantical.c")
list(APPEND ${name}.headers "dx/adl/semantical.h")
list(APPEND ${name}.sources "dx/adl/semantical/state.c")
list(APPEND ${name}.headers "dx/adl/semantical/state.h")
list(APPEND ${name}.sources "dx/adl/semantical/names.c")
list(APPEND ${name}.headers "dx/adl/semantical/names.h")
list(APPEND ${name}.inlays "dx/adl/semantical/names.i")
list(APPEND ${name}.sources "dx/adl/semantical/read.c")
list(APPEND ${name}.headers "dx/adl/semantical/read.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.headers})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.inlays})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.asms})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.readmes})

project(${name})

add_library(${name} ${${name}.sources} ${${name}.headers} ${${name}.readmes} ${${name}.inlays} ${${name}.asms})

# See ${CMAKE_SOURCE_DIR_DIR}/CMakeLibrary.cmake for more information.
dx_configure_warnings(${name})

target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${name} "dailies.core")

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
