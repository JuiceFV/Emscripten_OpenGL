# set the minimal version of the cmake
cmake_minimum_required(VERSION 3.9)

#============================= Main application information =========================# 
# set the name of the project 
# In the VS the main project calls as Emscripten_Graphics.sln
project(Emscripten_Graphics)

# Showing the system type
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")

# setting the version of the project
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}")

# Output the version of the project
message("${PROJECT_NAME} version: ${${PROJECT_NAME}_VERSION}")
#====================================================================================# 

#============================= Set Main Variables ===================================# 
# Set cmake's modules path
set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake/Modules
)

# set output dirictories dedicated for libs and binaries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Adding options (flags)
# 1) The flag, which has to be ON if we compile via emcc
option(FOR_EMSDK 
        "The libraries compilation occurs for the emsdk compilers" 
        OFF
    )

# Set the variables containing specific files.
set(${PROJECT_NAME}_SOURCES 
                "application/sources/shader.cpp"
                "application/sources/camera.cpp"
                "application/sources/texture.cpp"
                "application/sources/application.cpp"
                "application/sources/mesh.cpp"
                "application/sources/model.cpp"
                "application/sources/light.cpp"
                "application/sources/filedialog.cpp"
            )
set(${PROJECT_NAME}_INCLUDES 
                "application/includes/shader.h"
                "application/includes/camera.h"
                "application/includes/texture.h"
                "application/includes/vertex.h"
                "application/includes/application.h"
                "application/includes/mesh.h"
                "application/includes/light.h"
                "application/includes/model.h"
                "application/includes/globals.h"
                "application/includes/filedialog.h"
            )
set(${PROJECT_NAME}_MAIN "application/main.cxx")

# I've moved the group's functions to the another .cmake file.
include(groupfunctions)

# Look for additional files related to the program
# 1. The shaders
# 2. The images (especially for develop process), after I will remove this
# 3. The models
file(GLOB_RECURSE SHADERS ${CMAKE_SOURCE_DIR}/assets/shaders/*)
file(GLOB_RECURSE IMAGES ${CMAKE_SOURCE_DIR}/assets/images/*)
file(GLOB_RECURSE MODELS ${CMAKE_SOURCE_DIR}/assets/models/*)

# Group files for the proper representation in Visual Studio
# I leave models, because its object file may confront with VS
if(MSVC)
	assign_source_group(${SHADERS})
	assign_source_group(${IMAGES})
	source_group("" FILES ${${PROJECT_NAME}_MAIN})
endif()
#====================================================================================# 

#============================= Include additional cmake-files =======================# 
# 1) Set compilation's flags
# 2) Safe guards against in-source builds and bad build types.
# 3) Including file looking for libraries for the graphics.
include(setflags)
include(safeguard)
include(graphics)
#====================================================================================# 
#============================= Generate executable ==================================# 
message(STATUS "Executable mode is on!")

# adding executable
add_executable(${PROJECT_NAME} 
                ${${PROJECT_NAME}_MAIN} 
                ${${PROJECT_NAME}_SOURCES}
                ${${PROJECT_NAME}_INCLUDES}
                ${SHADERS}
                ${IMAGES}
            )

# Linking required libs
target_link_libraries(${PROJECT_NAME} 
                        ${GRAPHICS_LIBS}
                    ) 

# Set the include dirs as Interface
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/application/includes>
    $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/application/includes>
) 

if (NOT FOR_EMSDK)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
endif()

include(copydlls)
# Set some properties related only with emsdk
# Specifically, we creates some output and supporting dirictories:                           
#+-------------------------------------------------------------------------------------------+
#| To begin with, I tie volumes in the docker-compose and for these purposes                 |
#| I pass executable (.html, .js, .wasm, .data) to the tied directory. So that we            |   
#| may acquire those files on our host machine.                                              |
#+-------------------------------------------------------------------------------------------+
#| 2&3) Due to, whereas building occurs in the docker container, I created so-called warapper|
#| in python, which lunches a server and handles requests if those appears. Thus I have to   |
#| shove the entry.py to the binary dir, in purpose to launch it properly.                   |
#| Also, to make it possible to select a model at build time, I created my own .html file    |
#| and connected it to the generated .js.                                                    |
#+-------------------------------------------------------------------------------------------+
#| I'd like to emphasize the problem appears when the compilation occurs via emcc.           |
#| Emscripten creates its own filesystem and assign the start-dir as the root one.           |
#| In c++ code I have to step back to the previous directory to derive the assets/*          |
#| (../assets/models/*) if project was built via msvc-compiler, however for the emcc it      |
#| looks like (/assets/models/*). I do not really want to do a mess from the code,           |
#| therefore I copy the assets files to bin dir to make paths equal.                         |
#+-------------------------------------------------------------------------------------------+
if (FOR_EMSDK) 
    set_target_properties(${PROJECT_NAME}
                PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/application/out")
    configure_file(${CMAKE_SOURCE_DIR}/application/entry.py 
                    ${CMAKE_BINARY_DIR}/application/entry.py 
                    COPYONLY
                )
    configure_file(${CMAKE_SOURCE_DIR}/application/out/Emscripten_Graphics.html 
                    ${CMAKE_BINARY_DIR}/application/out/Emscripten_Graphics.html
                    COPYONLY
                )
    configure_file(${CMAKE_SOURCE_DIR}/application/out/Emscripten_Graphics.css 
                    ${CMAKE_BINARY_DIR}/application/out/Emscripten_Graphics.css
                    COPYONLY
                )
    configure_file(${CMAKE_SOURCE_DIR}/application/out/canvas.html 
                    ${CMAKE_BINARY_DIR}/application/out/canvas.html 
                    COPYONLY
                )
    configure_file(${CMAKE_SOURCE_DIR}/application/out/canvas.css 
                    ${CMAKE_BINARY_DIR}/application/out/canvas.css 
                    COPYONLY
                )
    configure_file(${CMAKE_SOURCE_DIR}/application/out/canvas.js 
                    ${CMAKE_BINARY_DIR}/application/out/canvas.js 
                    COPYONLY
                )
    configure_file(${CMAKE_SOURCE_DIR}/application/out/about.html 
                    ${CMAKE_BINARY_DIR}/application/out/about.html  
                    COPYONLY
                )
    configure_file(${CMAKE_SOURCE_DIR}/application/out/contacts.html 
                    ${CMAKE_BINARY_DIR}/application/out/contacts.html  
                    COPYONLY
                )
    configure_file(${CMAKE_SOURCE_DIR}/application/out/others.css 
                    ${CMAKE_BINARY_DIR}/application/out/others.css
                    COPYONLY
                )
else()
    copy_files(${CMAKE_SOURCE_DIR}/assets/* ${CMAKE_BINARY_DIR}/assets/*)
endif()

# If msvs then set the Emscripten_Graphics as startup project 
if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set ${PROJECT_NAME} as StartUp Project in Visual Studio.\n" )
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    endif()
endif()

# If the application is builded via emcc sets appropriate flags
if (FOR_EMSDK)
    message(STATUS "Setting EMCC flags\n")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s FORCE_FILESYSTEM=1 -s USE_WEBGL2=1 -s USE_GLFW=3  -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -s WASM=1 --bind -std=c++11 --preload-file ${CMAKE_SOURCE_DIR}/assets/@/assets")
endif()
#====================================================================================# 