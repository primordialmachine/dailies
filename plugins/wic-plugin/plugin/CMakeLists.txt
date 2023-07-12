# Copyright (c) 2023 Michael Heilmann. All rights reserved.
# Author: Michael Heilmann (michaelheilmann@primordialmachine.com
# Copyright: Copyright (c) 2023 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)


set(name "wic-plugin")

set (${name}.sources "")
set (${name}.headers "")

list(APPEND ${name}.sources "implementation/write-image.c")
list(APPEND ${name}.sources "implementation/read-image.c")

list(APPEND ${name}.sources "implementation/common.c")
list(APPEND ${name}.headers "implementation/common.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.sources} ${${name}.headers})

project(${name})

add_library(${name} MODULE ${${name}.sources} ${${name}.headers})

# See ${CMAKE_SOURCE_DIR_DIR}/CMakeLibrary.cmake for more information.
dx_configure_warnings(${name})

target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/implementation)
