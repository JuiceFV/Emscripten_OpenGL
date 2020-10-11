set(TESTS_INCLUDES "configloader_test.hpp")

add_executable(tests main.cxx ${TESTS_INCLUDES})
target_link_libraries(tests gtest_main gmock_main ConfigLoader ${LIBS} ${DEP_LIBS} ${DEP_EMSDK_LIBS})

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

set_target_properties(gmock gmock_main gtest gtest_main ${LIBS} ${DEP_LIBS} ${DEP_EMSDK_LIBS} PROPERTIES FOLDER "dependencies")