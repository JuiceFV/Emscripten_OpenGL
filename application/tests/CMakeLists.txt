# Building googletests
add_subdirectory(${CMAKE_SOURCE_DIR}/application/tests/dependencies/googletest)

set(TESTS_INCLUDES 
        "configloader_test.hpp"
)

add_executable(tests main.cxx ${TESTS_INCLUDES})
target_link_libraries(tests gtest_main gmock_main ${LIBS}
                            ${GRAPHICS_LIBS})

if (NOT FOR_EMSDK)
    find_package(OpenGL REQUIRED)
    target_link_libraries(tests OpenGL::GL)
endif()

add_test(
  NAME
    tests
  COMMAND
    $<TARGET_FILE:tests>
)

set_target_properties(gmock gmock_main gtest gtest_main ${LIBS} PROPERTIES FOLDER "dependencies")

message(STATUS "Setting up copy yamlparser dll..." "${CMAKE_BINARY_DIR}/bin/Debug/")
add_custom_command(TARGET tests
POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    "${CMAKE_SOURCE_DIR}/application/configloader/dependencies/yamlparser/lib/msvc/yaml-cppd.dll"      
    $<TARGET_FILE_DIR:tests>)