# GLAD
add_subdirectory(glad)
target_link_libraries(gl-tutorial glad)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
target_link_libraries(gl-tutorial glfw)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(gl-tutorial OpenGL::GL)

# STB
add_subdirectory(stb)
target_link_libraries(gl-tutorial stb)
