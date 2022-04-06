function(configure_chapter CHAPTER_NAME)
    add_executable(${CHAPTER_NAME} ../common/application.h
                                   ../common/application.cpp
                                   ../common/entry_point.h
                                   ../common/camera.h
                                   ../common/camera.cpp
                                   ../common/pipeline.h
                                   ../common/pipeline.cpp
    )

    target_include_directories(${CHAPTER_NAME} PRIVATE ../common/)

    target_link_libraries(${CHAPTER_NAME} glad)
    target_link_libraries(${CHAPTER_NAME} glfw)
    target_link_libraries(${CHAPTER_NAME} stb)
    target_link_libraries(${CHAPTER_NAME} glm)

    find_package(OpenGL REQUIRED)
    target_link_libraries(${CHAPTER_NAME} OpenGL::GL)

    add_dependencies(${CHAPTER_NAME} assets)
    add_dependencies(${CHAPTER_NAME} shaders)

    set_target_properties(${CHAPTER_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
                                              FOLDER "Chapters")
endfunction(configure_chapter)


add_subdirectory(getting-started)
add_subdirectory(lighting)