# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.ddl")


set(${name}.inlays "")
set(${name}.sources "")
set(${name}.headers "")
set(${name}.readmes "")

# "README.md"
list(APPEND ${name}.readmes "README.md")

# "ddl.(c|h)" files & "ddl" folder.
list(APPEND ${name}.sources "dx/ddl/word_kind.c")
list(APPEND ${name}.headers "dx/ddl/word_kind.h")
list(APPEND ${name}.inlays "dx/ddl/word_kind.i")
list(APPEND ${name}.sources "dx/ddl/word.c")
list(APPEND ${name}.headers "dx/ddl/word.h")
list(APPEND ${name}.sources "dx/ddl/scanner.c")
list(APPEND ${name}.headers "dx/ddl/scanner.h")
list(APPEND ${name}.sources "dx/ddl/node_kind.c")
list(APPEND ${name}.headers "dx/ddl/node_kind.h")
list(APPEND ${name}.inlays "dx/ddl/node_kind.i")
list(APPEND ${name}.sources "dx/ddl/node.c")
list(APPEND ${name}.headers "dx/ddl/node.h")
list(APPEND ${name}.sources "dx/ddl/parser.c")
list(APPEND ${name}.headers "dx/ddl/parser.h")
list(APPEND ${name}.sources "dx/ddl.c")
list(APPEND ${name}.headers "dx/ddl.h")
list(APPEND ${name}.sources "dx/ddl/diagnostics.c")
list(APPEND ${name}.headers "dx/ddl/diagnostics.h")
list(APPEND ${name}.sources "dx/ddl/position.c")
list(APPEND ${name}.headers "dx/ddl/position.h")
list(APPEND ${name}.sources "dx/ddl/linemap.c")
list(APPEND ${name}.headers "dx/ddl/linemap.h")

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
