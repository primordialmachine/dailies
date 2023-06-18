# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.day10_to_day14")


set(${name}.inlays "")
set(${name}.sources "")
set(${name}.headers "")
set(${name}.readmes "")

# "dx/application.(c|h)"
list(APPEND ${name}.sources "dx/application.c")
list(APPEND ${name}.headers "dx/application.h")

# "val" folder.
list(APPEND ${name}.sources "dx/val/cbinding.c" "dx/val/vbinding.c")
list(APPEND ${name}.headers "dx/val/cbinding.h" "dx/val/vbinding.h")
list(APPEND ${name}.sources "dx/val/program_text.c" "dx/val/command.c" "dx/val/program.c" "dx/val/context.c" "dx/val/buffer.c")
list(APPEND ${name}.headers "dx/val/program_text.h" "dx/val/command.h" "dx/val/program.h" "dx/val/context.h" "dx/val/buffer.h")

# "asset" folder.
list(APPEND ${name}.sources "dx/asset/mesh.c" "dx/asset/mesh_instance.c" "dx/asset/mesh/generators.c")
list(APPEND ${name}.headers "dx/asset/mesh.h" "dx/asset/mesh_instance.h" "dx/asset/mesh/generators.h")

list(APPEND ${name}.sources "dx/asset/scene.c")
list(APPEND ${name}.headers "dx/asset/scene.h")

list(APPEND ${name}.sources "dx/asset/texture.c" "dx/asset/material.c")
list(APPEND ${name}.headers "dx/asset/texture.h" "dx/asset/material.h")

list(APPEND ${name}.sources "dx/asset/image.c" "dx/asset/palette.c")
list(APPEND ${name}.headers "dx/asset/image.h" "dx/asset/palette.h")

# "adl" folder.
list(APPEND ${name}.sources "dx/adl/syntactical.c")
list(APPEND ${name}.headers "dx/adl/syntactical.h")

list(APPEND ${name}.sources "dx/adl/diagnostics.c")
list(APPEND ${name}.headers "dx/adl/diagnostics.h")

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.headers})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.readmes})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.asms})

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
