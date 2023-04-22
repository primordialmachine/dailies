# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "tif-writer")

set(${name}.sources "main.c")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources})

project(${name})

add_library(${name} MODULE ${${name}.sources})

if(MSVC)
  # Treat certain warnings as errors.

  # 'function' : different 'const' qualifiers
  target_compile_options(${name} PRIVATE "/we4090")
  # 'function' incompatible types - from 'xxx' to 'yyy'
  target_compile_options(${name} PRIVATE "/we4133")
  # 'xxx': too many actual parameters
  target_compile_options(${name} PRIVATE "/we4020")
  # 'xxx' undefined; assuming extern returning int
  target_compile_options(${name} PRIVATE "/we4013")

endif()

target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
