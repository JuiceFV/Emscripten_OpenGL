# set the minimal version of the cmake
cmake_minimum_required(VERSION 3.10)

# set the name of the project
project(ConfigLoader)

message(STATUS "Building the YAML parser")
option(YAML_CPP_BUILD_CONTRIB OFF)
option(YAML_CPP_BUILD_TOOLS OFF)
option(YAML_BUILD_SHARED_LIBS ON)
add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/yamlprsr)
message(STATUS "YAML parser - Done!\n")

set(${PROJECT_NAME}_sources "sources/config.cpp")
set(${PROJECT_NAME}_includes "includes/config.h")


add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_sources} ${${PROJECT_NAME}_includes})
target_link_libraries(${PROJECT_NAME} yaml-cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/includes")

add_compile_definitions(PATH_TO_HEADER="${PROJECT_SOURCE_DIR}/includes/config.h")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(yaml-cpp PROPERTIES FOLDER "dependencies")