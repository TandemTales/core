cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(core_standalone LANGUAGES CXX)

# --- Import tools ----
include(../cmake/tools.cmake)

# ---- Dependencies ----
include(../cmake/CPM.cmake)

CPMAddPackage(NAME core SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create standalone executable ----
file(GLOB sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

add_executable(${PROJECT_NAME} ${sources})
set_target_properties(${PROJECT_NAME} PROPERTIES 
    CXX_STANDARD 20
    OUTPUT_NAME "core"
)

target_link_libraries(${PROJECT_NAME} core)

# This line will create a folder hierarchy in the Visual Studio solution explorer
# that matches the directory structure under ${CMAKE_CURRENT_SOURCE_DIR}/source.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" FILES ${sources})