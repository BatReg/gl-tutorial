add_executable(lighting ../common/pipeline.h
                        ../common/pipeline.cpp
                        ../common/camera.h
                        ../common/camera.cpp
                        main.cpp
)

target_include_directories(lighting PRIVATE ../common/)

target_link_libraries(lighting glad)
target_link_libraries(lighting glfw)
target_link_libraries(lighting stb)
target_link_libraries(lighting glm)

find_package(OpenGL REQUIRED)
target_link_libraries(lighting OpenGL::GL)

add_dependencies(lighting assets)
add_dependencies(lighting shaders)

set_target_properties(lighting PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
                                                 FOLDER "Chapters")
