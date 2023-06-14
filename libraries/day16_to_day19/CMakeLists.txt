# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.day16today19")


set(${name}.dx.inlays "")
set(${name}.dx.sources "")
set(${name}.dx.headers "")

# configuration include and configuration source file.
list(APPEND ${name}.dx.sources "dx/configuration.c")
list(APPEND ${name}.dx.headers "dx/configuration.h")

# master include file and master source file.
list(APPEND ${name}.dx.sources "dx/core.c")
list(APPEND ${name}.dx.headers "dx/core.h")

# "core" folder.
list(APPEND ${name}.dx.sources "dx/core/core.c" "dx/core/os.c" "dx/core/string.c")
list(APPEND ${name}.dx.headers "dx/core/core.h" "dx/core/os.h" "dx/core/string.h")
list(APPEND ${name}.dx.sources "dx/core/math.c")
list(APPEND ${name}.dx.headers "dx/core/math.h")
list(APPEND ${name}.dx.sources "dx/core/object_array.c" "dx/core/byte_array.c")
list(APPEND ${name}.dx.headers "dx/core/object_array.h" "dx/core/byte_array.h")
list(APPEND ${name}.dx.sources "dx/core/pointer_array.c")
list(APPEND ${name}.dx.headers "dx/core/pointer_array.h")
list(APPEND ${name}.dx.sources "dx/core/pointer_hashmap.c")
list(APPEND ${name}.dx.headers "dx/core/pointer_hashmap.h")
list(APPEND ${name}.dx.sources "dx/core/file_system.c")
list(APPEND ${name}.dx.headers "dx/core/file_system.h")
list(APPEND ${name}.dx.sources "dx/core/visuals.c")
list(APPEND ${name}.dx.headers "dx/core/visuals.h")
list(APPEND ${name}.dx.sources "dx/core/safe_add_nx.c" "dx/core/safe_mul_ix.c" "dx/core/safe_mul_nx.c")
list(APPEND ${name}.dx.headers "dx/core/safe_add_nx.h" "dx/core/safe_mul_ix.h" "dx/core/safe_mul_nx.h")
list(APPEND ${name}.dx.sources "dx/core/count_leading_zeroes.c")
list(APPEND ${name}.dx.headers "dx/core/count_leading_zeroes.h")
list(APPEND ${name}.dx.sources "dx/core/next_power_of_two.c")
list(APPEND ${name}.dx.headers "dx/core/next_power_of_two.h")
list(APPEND ${name}.dx.sources "dx/core/convert.c")
list(APPEND ${name}.dx.headers "dx/core/convert.h")

list(APPEND ${name}.dx.sources "dx/core/canvas_msgs.c")
list(APPEND ${name}.dx.headers "dx/core/canvas_msgs.h")

list(APPEND ${name}.dx.sources "dx/core/hapticals.c")
list(APPEND ${name}.dx.headers "dx/core/hapticals.h")
list(APPEND ${name}.dx.inlays "dx/core/keyboard_keys.i")
list(APPEND ${name}.dx.inlays "dx/core/mouse_buttons.i")

# "asset" folder.
list(APPEND ${name}.dx.sources "dx/asset/mesh.c" "dx/asset/mesh_instance.c" "dx/asset/mesh/generators.c")
list(APPEND ${name}.dx.headers "dx/asset/mesh.h" "dx/asset/mesh_instance.h" "dx/asset/mesh/generators.h")

list(APPEND ${name}.dx.sources "dx/asset/scene.c")
list(APPEND ${name}.dx.headers "dx/asset/scene.h")

list(APPEND ${name}.dx.sources "dx/asset/texture.c" "dx/asset/material.c")
list(APPEND ${name}.dx.headers "dx/asset/texture.h" "dx/asset/material.h")

list(APPEND ${name}.dx.sources "dx/asset/image.c" "dx/asset/palette.c")
list(APPEND ${name}.dx.headers "dx/asset/image.h" "dx/asset/palette.h")

# "adl" folder.
list(APPEND ${name}.dx.sources "dx/adl/syntactical.c")
list(APPEND ${name}.dx.headers "dx/adl/syntactical.h")

list(APPEND ${name}.dx.sources "dx/adl/diagnostics.c")
list(APPEND ${name}.dx.headers "dx/adl/diagnostics.h")

list(APPEND ${name}.dx.sources "dx/adl/semantical.c")
list(APPEND ${name}.dx.headers "dx/adl/semantical.h")
list(APPEND ${name}.dx.sources "dx/adl/semantical/state.c")
list(APPEND ${name}.dx.headers "dx/adl/semantical/state.h")

# "asm" folder.
# Assembler sources.
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
add_files( ${name}.dx.inlays "dx/asm/${infix}/_umul*.i")
add_files( ${name}.dx.sources "dx/asm/${infix}/_umul*.c" )
add_files( ${name}.dx.headers "dx/asm/${infix}/_umul*.h" )
add_files( ${name}.dx.asms "dx/asm/${infix}/_umul*.asm" )
#
add_files( ${name}.dx.inlays "dx/asm/${infix}/_imul*.i")
add_files( ${name}.dx.sources "dx/asm/${infix}/_imul*.c" )
add_files( ${name}.dx.headers "dx/asm/${infix}/_imul*.h" )
add_files( ${name}.dx.asms "dx/asm/${infix}/_imul*.asm" )
#
if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  set_source_files_properties(
      ${${name}.dx.asms}
      PROPERTIES
      COMPILE_FLAGS "/safeseh"
  )
endif()

set(${name}.sources ${${name}.dx.sources} ${${name}.dx.gl.sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources})

set(${name}.headers ${${name}.dx.headers} ${${name}.dx.gl.headers})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.headers})

set(${name}.readmes "README.md")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.readmes})

set(${name}.asms ${${name}.dx.asms})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.asms})

project(${name})

add_library(${name} ${${name}.sources} ${${name}.headers} ${${name}.readmes} ${${name}.inlays} ${${name}.asms})

if(MSVC)
  # Treat certain warnings as errors.

  # 'function' : different 'const' qualifiers
  set_source_files_properties(${${name}.sources} PROPERTIES COMPILE_OPTIONS "/we4090")
  set_source_files_properties(${${name}.headers} PROPERTIES COMPILE_OPTIONS "/we4090")
  # 'function' incompatible types - from 'xxx' to 'yyy'
  #target_compile_options(${name} PRIVATE "/we4133")
  set_source_files_properties(${${name}.sources} PROPERTIES COMPILE_OPTIONS "/we4133")
  set_source_files_properties(${${name}.headers} PROPERTIES COMPILE_OPTIONS "/we4133")
  # 'xxx': too many actual parameters
  set_source_files_properties(${${name}.sources} PROPERTIES COMPILE_OPTIONS "/we4020")
  set_source_files_properties(${${name}.headers} PROPERTIES COMPILE_OPTIONS "/we4020")
  # 'xxx' undefined; assuming extern returning int
  set_source_files_properties(${${name}.sources} PROPERTIES COMPILE_OPTIONS "/we4013")
  set_source_files_properties(${${name}.headers} PROPERTIES COMPILE_OPTIONS "/we4013")

endif()

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
