# set the minimal version of the cmake
cmake_minimum_required(VERSION 3.10)

# set the name of the project
project(Emscripten_Graphics)

# Showing the system type
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")

# setting the version of the project
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}")

# Output the version of the project
message("${PROJECT_NAME} version: ${${PROJECT_NAME}_VERSION}")

# Set cmake's modules path
set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake/Modules
)

# variables for the library
# stack source-files
set(${PROJECT_NAME}_SOURCES "application/sources/a.cxx")
set(${PROJECT_NAME}_INCLUDES "application/includes/a.hpp")
set(${PROJECT_NAME}_MAIN "application/main.cxx")

# Check for in-source builds.
include(safeguard)

# Set compilation's flags
include(setflags)

include(Findglfw3)

message(STATUS "Executable mode is on!")

# adding executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_MAIN})
target_link_libraries(${PROJECT_NAME} glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)