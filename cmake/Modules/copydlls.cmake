 if(WIN32)
 	#copy the dll file to the executable directory
    message(STATUS "Setting up copy glew dll..." "${CMAKE_BINARY_DIR}/bin/Debug/")
    add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${CMAKE_SOURCE_DIR}/application/dependencies/glew/lib/msvc/glewd.dll"      
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

    message(STATUS "Setting up copy glfw dll..." "${CMAKE_BINARY_DIR}/bin/Debug/")
    add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${CMAKE_SOURCE_DIR}/application/dependencies/glfw/lib/msvc/glfw3.dll"      
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

    message(STATUS "Setting up copy assimp dll..." "${CMAKE_BINARY_DIR}/bin/Debug/")
    add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${CMAKE_SOURCE_DIR}/application/dependencies/assimp/lib/msvc/assimp-vc142-mtd.dll"      
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()