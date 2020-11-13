message(STATUS "Building the raphics libraries...\n")

# Split over compilers
if (MSVC)

# Looking for assimp library 
find_library(assimp 
			 NAMES assimp-vc142-mtd.lib 
			 PATHS ${CMAKE_SOURCE_DIR}/application/dependencies/assimp/lib/msvc
)

# Looking for soil2 library 
find_library(soil2 
			 NAMES soil2.lib
			 PATHS ${CMAKE_SOURCE_DIR}/application/dependencies/soil2/lib/msvc
)

# Looking for glew (msvc only) library 
find_library(glew 
			 NAMES glewd.lib
			 PATHS ${CMAKE_SOURCE_DIR}/application/dependencies/glew/lib/msvc
)

# Looking for glfw (msvc only) library 
find_library(glfw 
			 NAMES glfw3dll.lib
			 PATHS ${CMAKE_SOURCE_DIR}/application/dependencies/glfw/lib/msvc
)
else()

# I don't aware why does it happen, but 
# in some reason if I do not set these two vars
# cmake is not able to find the libs on linux
# here the issue https://github.com/mxe/mxe/issues/2018
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)

# Looking for assimp library 
find_library(assimp 
			 NAMES assimp
			 PATHS ${CMAKE_SOURCE_DIR}/application/dependencies/assimp/lib/emsdk
)

# Looking for soil2 library 
find_library(soil2 
			 NAMES soil2
			 PATHS ${CMAKE_SOURCE_DIR}/application/dependencies/soil2/lib/emsdk
)
endif()


# Linking include's directories of the found libs
# and appending them to the GRAPHICS_LIBS list.
if(MSVC)
	if (glew)
		message(STATUS "Looking for the GLEW")
		message("GLEW has been found!")
		list(APPEND GRAPHICS_LIBS ${glew})
		include_directories(${CMAKE_SOURCE_DIR}/application/dependencies/glew/include)
		message(STATUS "GLEW - Done!\n")
	endif(glew)
	if (glfw)
		message(STATUS "Looking for the GLFW")
		message("GLFW has been found!")
		list(APPEND GRAPHICS_LIBS ${glfw})
		include_directories(${CMAKE_SOURCE_DIR}/application/dependencies/glfw/include)
		message(STATUS "GLFW - Done!\n")
	endif(glfw)
endif(MSVC)

message(STATUS "Looking for the GLM")
message("GLM has been found!")
include_directories(${CMAKE_SOURCE_DIR}/application/dependencies/glm)
message(STATUS "GLM - Done!\n")

message(STATUS "Looking for the SOIL")
if (soil2)
message("SOIL2 has been found!")
list(APPEND GRAPHICS_LIBS ${soil2})
include_directories(
	${CMAKE_SOURCE_DIR}/application/dependencies/soil2/include/SOIL2
	${CMAKE_SOURCE_DIR}/application/dependencies/soil2/include/common

	)
endif(soil2)
message(STATUS "SOIL2 - Done!\n")


message(STATUS "Looking for the ASSIMP")
if (assimp)
	message("Assimp has been found!")
	list(APPEND GRAPHICS_LIBS ${assimp})
	include_directories(${CMAKE_SOURCE_DIR}/application/dependencies/assimp/include)
endif()
message(STATUS "ASSIMP - Done!\n")

