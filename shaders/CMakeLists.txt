file(GLOB_RECURSE SHADER_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.vert
                                      ${CMAKE_CURRENT_SOURCE_DIR}/src/*.frag
)

foreach(SHADER ${SHADER_SOURCE_FILES})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(TARGET_SHADER_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/shaders/${FILE_NAME})

    add_custom_command(
        OUTPUT ${TARGET_SHADER_FILE}
        COMMAND ${CMAKE_COMMAND} -E 
            copy_if_different ${SHADER} ${TARGET_SHADER_FILE}
        DEPENDS ${SHADER}
        VERBATIM
    )

    list(APPEND SHADER_FILES ${TARGET_SHADER_FILE})
endforeach(SHADER ${SHADER_SOURCE_FILES})

add_custom_target(
    shaders ALL
    DEPENDS ${SHADER_FILES}
    SOURCES ${SHADER_SOURCE_FILES}
)
source_group("Source Files" FILES ${SHADER_SOURCE_FILES})

add_dependencies(gl-tutorial shaders)
