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

# https://stackoverflow.com/questions/45260216/emscripten-cmake-pass-emscripten-options-in-cmakelist-file
option(FOR_EMSDK "The libraries compilation occurs for the emsdk compilers" OFF)

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
if (NOT FOR_EMSDK)
    include(graphics)
endif()
include(configloader)


set(DEP_EMSDK_LIBS)

if (NOT FOR_EMSDK)
    set(DEP_LIBS
        glfw 
        libglew_shared
    )
endif()

set(LIBS 
    ConfigLoader
)

message(STATUS "Executable mode is on!")

# adding executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_MAIN})
target_link_libraries(${PROJECT_NAME} ${LIBS} ${DEP_LIBS} ${DEP_EMSDK_LIBS})  

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if (FOR_EMSDK)
    set_target_properties(${DEP_EMSDK_LIBS} PROPERTIES FOLDER "dependencies")
elseif(${DEP_EMSDK_LIBS})
    set_target_properties(${DEP_LIBS} PROPERTIES FOLDER "dependencies")
    set_target_properties(${DEP_EMSDK_LIBS} PROPERTIES FOLDER "dependencies")
else()
    set_target_properties(${DEP_LIBS} PROPERTIES FOLDER "dependencies")
endif()

if (NOT FOR_EMSDK)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
endif()

if (FOR_EMSDK)
    message(STATUS "Setting EMCC flags\n")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NODERAWFS=1 -s FORCE_FILESYSTEM=1 -s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1 -std=c++1z")
endif()

if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set ${PROJECT_NAME} as StartUp Project in Visual Studio.\n" )
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )
    endif()
endif()