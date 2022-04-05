add_executable(getting-started ../common/pipeline.h
                               ../common/pipeline.cpp
                               ../common/camera.h
                               ../common/camera.cpp
                               main.cpp
)

target_include_directories(getting-started PRIVATE ../common/)

target_link_libraries(getting-started glad)
target_link_libraries(getting-started glfw)
target_link_libraries(getting-started stb)
target_link_libraries(getting-started glm)

find_package(OpenGL REQUIRED)
target_link_libraries(getting-started OpenGL::GL)

add_dependencies(getting-started assets)
add_dependencies(getting-started shaders)

set_target_properties(getting-started PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
                                                 FOLDER "Chapters")
