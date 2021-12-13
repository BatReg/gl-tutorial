file(GLOB_RECURSE TEXTURE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/textures/*.png
                                ${CMAKE_CURRENT_SOURCE_DIR}/textures/*.jpg
)

foreach(TEXTURE_FILE ${TEXTURE_FILES})
    get_filename_component(TEXTURE_FILE_NAME ${TEXTURE_FILE} NAME)
    set(TARGET_TEXTURE_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/assets/textures/${TEXTURE_FILE_NAME})

    add_custom_command(
        OUTPUT ${TARGET_TEXTURE_FILE}
        COMMAND ${CMAKE_COMMAND} -E
            copy_if_different ${TEXTURE_FILE} ${TARGET_TEXTURE_FILE}
            DEPENDS ${TEXTURE_FILE}
            VERBATIM
    )

    list(APPEND TARGET_TEXTURE_FILES ${TARGET_TEXTURE_FILE})
endforeach(TEXTURE_FILE ${TEXTURE_FILES})

add_custom_target(
    assets ALL
    DEPENDS ${TARGET_TEXTURE_FILES}
    SOURCES ${TEXTURE_FILES}
)
source_group("Textures" FILES ${TEXTURE_FILES})

add_dependencies(gl-tutorial assets)
