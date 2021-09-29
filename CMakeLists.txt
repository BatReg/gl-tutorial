cmake_minimum_required(VERSION 3.13.0)

project(gl-tutorial)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT gl-tutorial)

add_subdirectory(src)
add_subdirectory(external)
