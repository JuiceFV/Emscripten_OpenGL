if (NOT FOR_EMSDK)
    find_package(OpenGL REQUIRED)
endif()

add_executable(configloader_test configloader_test.cpp)
target_link_libraries(configloader_test gtest_main ConfigLoader)

add_test(
  NAME
    unit
  COMMAND
    ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:configloader_test>
)

set_target_properties(gmock gmock_main gtest gtest_main PROPERTIES FOLDER "dependencies")