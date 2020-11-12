message(STATUS "Building the raphics libraries...\n")

if (NOT FOR_EMSDK)
	message(STATUS "Building the GLEW")
	option(glew-cmake_BUILD_STATIC OFF)
	option(ONLY_LIBS ON)
	add_subdirectory(${CMAKE_SOURCE_DIR}/application/dependencies/GLEW)
	message(STATUS "GLEW - Done!\n")
	
	message(STATUS "Building the GLFW")
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	option(BUILD_SHARED_LIBS ON)
	option(GLFW_INSTALL OFF)
	add_subdirectory(${CMAKE_SOURCE_DIR}/application/dependencies/GLFW)
	message(STATUS "GLFW - Done!\n")
endif()

message(STATUS "Building the GLM")
option(BUILD_STATIC_LIBS OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/application/dependencies/GLM)
message(STATUS "GLM - Done!\n")

message(STATUS "Building the SOIL2")
add_subdirectory(${CMAKE_SOURCE_DIR}/application/dependencies/SOIL2)
message(STATUS "SOIL2 - Done!\n")

# Further I'ill decide if I shall to use the Assimp
message(STATUS "Building the ASSIMP")
if (UNIX)
find_library(assimp NAMES assimp libassimp.a
			PATHS ${CMAKE_SOURCE_DIR}/application/dependencies/ASSIMP/build/code)	
message(${assimp})
else()
find_library(assimp NAMES assimp-vc142-mtd.lib 
			PATHS ${CMAKE_SOURCE_DIR}/application/dependencies/assimp_libs/lib/windows)
	list(APPEND COMMON_LIBS ${assimp})
	configure_file(${CMAKE_SOURCE_DIR}/application/dependencies/assimp_libs/lib/windows/assimp-vc142-mtd.dll 
                        ${CMAKE_BINARY_DIR}/bin/Debug/assimp-vc142-mtd.dll
                        COPYONLY
                    )
endif()
if (assimp)
message("Assimp has been found!")
include_directories(${CMAKE_SOURCE_DIR}/application/dependencies/assimp_libs/include)
endif()
#set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
#set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
#set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
#set(SYSTEM_IRRXML OFF CACHE BOOL "" FORCE)
#add_subdirectory(${CMAKE_SOURCE_DIR}/application/dependencies/ASSIMP)
message(STATUS "ASSIMP - Done!\n")

