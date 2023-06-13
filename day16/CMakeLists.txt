# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.day16")


# DX sources.
set(${name}.dx.inlays "")
set(${name}.dx.sources "")
set(${name}.dx.headers "")

list(APPEND ${name}.dx.sources "dx/application.c" "dx/configuration.c" "dx/scene.c")
list(APPEND ${name}.dx.headers "dx/application.h" "dx/configuration.h" "dx/scene.h")

# DX scene / create assets.
list(APPEND ${name}.dx.sources "dx/scenes/create_assets.c")
list(APPEND ${name}.dx.headers "dx/scenes/create_assets.h")

# DX core sources.
set(${name}.dx.core.inlays "")
set(${name}.dx.core.sources "")
set(${name}.dx.core.headers "")
list(APPEND ${name}.dx.sources "dx/core.c")
list(APPEND ${name}.dx.headers "dx/core.h")
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

# DX asset sources.
list(APPEND ${name}.dx.sources "dx/asset/mesh.c" "dx/asset/mesh_instance.c" "dx/asset/mesh/generators.c")
list(APPEND ${name}.dx.headers "dx/asset/mesh.h" "dx/asset/mesh_instance.h" "dx/asset/mesh/generators.h")

list(APPEND ${name}.dx.sources "dx/asset/scene.c")
list(APPEND ${name}.dx.headers "dx/asset/scene.h")

list(APPEND ${name}.dx.sources "dx/asset/texture.c" "dx/asset/material.c")
list(APPEND ${name}.dx.headers "dx/asset/texture.h" "dx/asset/material.h")

list(APPEND ${name}.dx.sources "dx/asset/image.c" "dx/asset/palette.c")
list(APPEND ${name}.dx.headers "dx/asset/image.h" "dx/asset/palette.h")

# DX ADL sources.
list(APPEND ${name}.dx.sources "dx/adl/syntactical.c")
list(APPEND ${name}.dx.headers "dx/adl/syntactical.h")

list(APPEND ${name}.dx.sources "dx/adl/diagnostics.c")
list(APPEND ${name}.dx.headers "dx/adl/diagnostics.h")

list(APPEND ${name}.dx.sources "dx/adl/semantical.c")
list(APPEND ${name}.dx.headers "dx/adl/semantical.h")
list(APPEND ${name}.dx.sources "dx/adl/semantical/state.c")
list(APPEND ${name}.dx.headers "dx/adl/semantical/state.h")

# DX visuals abstraction layer (VAL).
list(APPEND ${name}.dx.sources "dx/texture.c" "dx/material.c" "dx/mesh.c" "dx/mesh_instance.c")
list(APPEND ${name}.dx.headers "dx/texture.h" "dx/material.h" "dx/mesh.h" "dx/mesh_instance.h")
list(APPEND ${name}.dx.sources "dx/program_text.c" "dx/command.c" "dx/program.c" "dx/context.c" "dx/cbinding.c" "dx/vbinding.c" "dx/buffer.c")
list(APPEND ${name}.dx.headers "dx/program_text.h" "dx/command.h" "dx/program.h" "dx/context.h" "dx/cbinding.h" "dx/vbinding.h" "dx/buffer.h")

# DX VAL OpenGL backend.
set(${name}.dx.gl.sources "dx/gl/context.c" "dx/gl/wgl/context.c" "dx/gl/texture.c" "dx/gl/program.c" "dx/gl/buffer.c" "dx/gl/binding.c" "dx/gl/wgl/wm.c")
set(${name}.dx.gl.headers "dx/gl/context.h" "dx/gl/wgl/context.h" "dx/gl/texture.h" "dx/gl/program.h" "dx/gl/buffer.h" "dx/gl/binding.h" "dx/gl/wgl/wm.h" )

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

# DX scenes sources.
list(APPEND ${name}.dx.sources "dx/scenes/mesh_viewer_scene.c")
list(APPEND ${name}.dx.headers "dx/scenes/mesh_viewer_scene.h")

set(${name}.sources "main.c" ${${name}.dx.sources} ${${name}.dx.gl.sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources})

set(${name}.headers ${${name}.dx.headers} ${${name}.dx.gl.headers})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.headers})

set(${name}.readmes "README.md")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.readmes})

set(${name}.inlays ${${name}.dx.inlays} "dx/gl/functions.i")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.inlays})

set(${name}.asms ${${name}.dx.asms})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.asms})

project(${name})

add_executable(${name} ${${name}.sources} ${${name}.headers} ${${name}.readmes} ${${name}.inlays} ${${name}.asms})

# DLL plugins.
add_dependencies(${name} tif-writer)
add_custom_command(TARGET ${name} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${CMAKE_CURRENT_BINARY_DIR}/../tif-writer/$<CONFIG>/tif-writer.dll $<TARGET_FILE_DIR:${name}>
                   COMMAND_EXPAND_LISTS)

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

target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${CMAKE_CURRENT_SOURCE_DIR}/external/opengl/includes)

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
