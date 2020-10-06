message(STATUS "Building the GLEW and GLFW libraries...\n")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

message(STATUS "Building the GLFW")
option(BUILD_SHARED_LIBS ON)
option(GLFW_INSTALL OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/application/dependencies/GLFW)
message(STATUS "GLFW - Done!\n")

message(STATUS "Building the GLEW")
option(glew-cmake_BUILD_STATIC OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/application/dependencies/GLEW)
message(STATUS "GLEW - Done!\n")

