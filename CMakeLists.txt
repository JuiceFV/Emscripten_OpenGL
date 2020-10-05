# set the minimal version of the cmake
cmake_minimum_required(VERSION 3.1)

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

# Set the variables responsible for specific files.
set(${PROJECT_NAME}_SOURCES)
set(${PROJECT_NAME}_INCLUDES)
set(${PROJECT_NAME}_MAIN "application/main.cxx")

# Include additional cmake-files: 
# 1) Set compilation's flags
# 2) Safe guards against in-source builds and bad build types.
# 3) Including file looking for libraries for the graphics
include(setflags)
include(safeguard)
include(graphics)

message(STATUS "Executable mode is on!")

# adding executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_MAIN})
target_link_libraries(${PROJECT_NAME} glfw libglew_shared)


find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set ${PROJECT_NAME} as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )
    endif()
endif()