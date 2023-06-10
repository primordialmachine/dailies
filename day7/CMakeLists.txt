# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "dailies.day7")


# DX sources.
set(${name}.dx.sources "dx/application.c" "dx/configuration.c" "dx/core.c")
set(${name}.dx.headers "dx/application.h" "dx/configuration.h" "dx/core.h")

# DX OpenGL backend sources.
set(${name}.dx.gl.sources "dx/gl/wgl/wm.c")
set(${name}.dx.gl.headers "dx/gl/wgl/wm.h")

set(${name}.sources "main.c" ${${name}.dx.sources} ${${name}.dx.gl.sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources})

set(${name}.headers ${${name}.dx.headers} ${${name}.dx.gl.headers})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.headers})

set(${name}.readmes "README.md")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.readmes})

project(${name})

add_executable(${name} ${${name}.sources} ${${name}.headers} ${${name}.readmes})

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
