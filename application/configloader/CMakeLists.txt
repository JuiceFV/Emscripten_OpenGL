# This CMakeLists represents rules intended to build the library
# responsible for configuration handler.
# set the minimal version of the cmake
cmake_minimum_required(VERSION 3.9)

# set the name of the project
project(ConfigLoader)

message(STATUS "Looking for the YAML parser")
if(MSVC)
find_library(yaml-cpp 
			 NAMES yaml-cppd.lib
			 PATHS ${CMAKE_SOURCE_DIR}/application/configloader/dependencies/yamlparser/lib/msvc
)
else()
find_library(yaml-cpp 
			 NAMES yaml-cpp
			 PATHS ${CMAKE_SOURCE_DIR}/application/configloader/dependencies/yamlparser/lib/emsdk
)
endif(MSVC)

set(${PROJECT_NAME}_sources "sources/config.cpp")
set(${PROJECT_NAME}_includes 
                    "includes/config.h"
                    "includes/globals.h"
                    )


add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_sources} ${${PROJECT_NAME}_includes})
target_link_libraries(${PROJECT_NAME} ${yaml-cpp})
target_include_directories(
    ${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/includes>
)

if(yaml-cpp)
	message("YAML parser has been find!")
	target_include_directories(
    ${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/application/configloader/dependencies/yamlparser/include
)
endif()
message(STATUS "YAML parser - Done!\n")
