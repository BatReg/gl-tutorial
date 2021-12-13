project(Stb)

add_library(stb INTERFACE include/stb/stb_image.h)

target_include_directories(stb INTERFACE include/)
target_compile_definitions(stb INTERFACE STB_IMAGE_IMPLEMENTATION)