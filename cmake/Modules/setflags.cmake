  
# set flags
# Here I define the flags for different compilers.

# First, print the message that CMake setting flags.
message(STATUS "Set flags for the compiler")

if(CMAKE_CXX_COMPILER_ID MATCHES MINGW)
    # In case if compiler is mingw
    message(STATUS "Seting flags for the MINGW Compiler")
    # Setting std to refer whether to the draft of c++14 standard and to the draft of c++20 standard.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -std=c++2a")
    # Next, setting -Wall (which tells the compiler to send all warning messages to the screen)
    # and -O3 (which approximately high optimizes a compilation, more info: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")
    # For debug -O0 (Reduce compilation time and make debugging produce the expected results. This is the default.)
    # and -g turns debug information on (default debug information).
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")
    # For release optimize the compilation
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	message(STATUS "Flags for the MINGW Compiler was set")
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    # In case if compiler is msvc
    message(STATUS "Seting flags for the MSVC Compiler")
    # setting the latest version of c++.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
    # setting warning lagging and compilation's optimization.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /O2")
    # /Od - turning off all optimizations in a program and making compilation faster.
    # /Zi - writes all logs into PDB-file.
    set(CMAKE_CXX_FLAGS_DEBUG   "/Od /DEBUG /Zi")
    # /favor - optimizes a code for a specific architecture or for the specifics of microarchitectures in AMD and Intel architectures.
	set(CMAKE_CXX_FLAGS_RELEASE "/favor")
	message(STATUS "Flags for the MSVC Compiler was set")
elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    # In case if compiler is gnu the flags the same as in mingw section.
	message(STATUS "Seting flags for the GNU Compiler")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -std=c++2a")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	message(STATUS "Flags for the GNU Compiler was set")
endif()