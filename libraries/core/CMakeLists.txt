# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.core")


set(${name}.inlays "")
set(${name}.sources "")
set(${name}.headers "")
set(${name}.readmes "")

# README.d.
list(APPEND ${name}.readmes "README.md")

# configuration files.
list(APPEND ${name}.sources "dx/core/configuration.c")
list(APPEND ${name}.headers "dx/core/configuration.h")

# master files.
list(APPEND ${name}.sources "dx/core.c")
list(APPEND ${name}.headers "dx/core.h")

# "core" folder.
list(APPEND ${name}.sources "dx/core/core.c" "dx/core/os.c")
list(APPEND ${name}.headers "dx/core/core.h" "dx/core/os.h")

list(APPEND ${name}.sources "dx/core/_string_format.c" "dx/core/string.c")
list(APPEND ${name}.headers "dx/core/_string_format.h" "dx/core/string.h")

list(APPEND ${name}.sources "dx/core/math.c")
list(APPEND ${name}.headers "dx/core/math.h")
list(APPEND ${name}.sources "dx/core/memory.c")
list(APPEND ${name}.headers "dx/core/memory.h")
list(APPEND ${name}.sources "dx/core/object_array.c" "dx/core/byte_array.c")
list(APPEND ${name}.headers "dx/core/object_array.h" "dx/core/byte_array.h")
list(APPEND ${name}.sources "dx/core/pointer_array.c")
list(APPEND ${name}.headers "dx/core/pointer_array.h")
list(APPEND ${name}.sources "dx/core/pointer_hashmap.c")
list(APPEND ${name}.headers "dx/core/pointer_hashmap.h")
list(APPEND ${name}.sources "dx/core/file_system.c")
list(APPEND ${name}.headers "dx/core/file_system.h")
list(APPEND ${name}.sources "dx/core/visuals.c")
list(APPEND ${name}.headers "dx/core/visuals.h")
list(APPEND ${name}.sources "dx/core/safe_add_nx.c" "dx/core/safe_mul_ix.c" "dx/core/safe_mul_nx.c")
list(APPEND ${name}.headers "dx/core/safe_add_nx.h" "dx/core/safe_mul_ix.h" "dx/core/safe_mul_nx.h")
list(APPEND ${name}.sources "dx/core/count_leading_zeroes.c")
list(APPEND ${name}.headers "dx/core/count_leading_zeroes.h")
list(APPEND ${name}.sources "dx/core/next_power_of_two.c")
list(APPEND ${name}.headers "dx/core/next_power_of_two.h")
list(APPEND ${name}.sources "dx/core/convert.c")
list(APPEND ${name}.headers "dx/core/convert.h")

list(APPEND ${name}.sources "dx/core/canvas_msgs.c")
list(APPEND ${name}.headers "dx/core/canvas_msgs.h")

list(APPEND ${name}.sources "dx/core/hapticals.c")
list(APPEND ${name}.headers "dx/core/hapticals.h")
list(APPEND ${name}.inlays "dx/core/keyboard_keys.i")
list(APPEND ${name}.inlays "dx/core/mouse_buttons.i")

list(APPEND ${name}.sources "dx/core/object.c")
list(APPEND ${name}.headers "dx/core/object.h")

list(APPEND ${name}.sources "dx/core/msgs.c")
list(APPEND ${name}.headers "dx/core/msgs.h")

# "core/asm" folder.
enable_language(ASM_MASM) # Enable ASM provided by MASM.

# Small utility function to find files by expression and append them to the specified list.
function(add_files file_list expression)
  file(GLOB_RECURSE temporary ${expression})
  set(${file_list} ${${file_list}} ${temporary} PARENT_SCOPE)
endfunction()

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
#
add_files( ${name}.inlays "dx/core/asm/${infix}/_umul*.i")
add_files( ${name}.sources "dx/core/asm/${infix}/_umul*.c" )
add_files( ${name}.headers "dx/core/asm/${infix}/_umul*.h" )
add_files( ${name}.asms "dx/core/asm/${infix}/_umul*.asm" )
#
add_files( ${name}.inlays "dx/core/asm/${infix}/_imul*.i")
add_files( ${name}.sources "dx/core/asm/${infix}/_imul*.c" )
add_files( ${name}.headers "dx/core/asm/${infix}/_imul*.h" )
add_files( ${name}.asms "dx/core/asm/${infix}/_imul*.asm" )
#
if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  set_source_files_properties(
      ${${name}.asms}
      PROPERTIES
      COMPILE_FLAGS "/safeseh"
  )
endif()

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
