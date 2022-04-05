file(GLOB_RECURSE SHADER_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.vert
                                      ${CMAKE_CURRENT_SOURCE_DIR}/src/*.frag
)

foreach(SHADER_FILE ${SHADER_SOURCE_FILES})
    get_filename_component(SHADER_FILE_NAME ${SHADER_FILE} NAME)
    set(TARGET_SHADER_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/shaders/${SHADER_FILE_NAME})

    add_custom_command(
        OUTPUT ${TARGET_SHADER_FILE}
        COMMAND ${CMAKE_COMMAND} -E 
            copy_if_different ${SHADER_FILE} ${TARGET_SHADER_FILE}
        DEPENDS ${SHADER_FILE}
        VERBATIM
    )

    list(APPEND TARGET_SHADER_FILES ${TARGET_SHADER_FILE})
endforeach(SHADER_FILE ${SHADER_SOURCE_FILES})

add_custom_target(
    shaders ALL
    DEPENDS ${TARGET_SHADER_FILES}
    SOURCES ${SHADER_SOURCE_FILES}
)
source_group("Source Files" FILES ${SHADER_SOURCE_FILES})
