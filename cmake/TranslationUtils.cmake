function(add_translation_resource out_file prefix)
    set(qm_files ${ARGN})
    set(resource ${CMAKE_CURRENT_SOURCE_DIR}/${prefix}.translations.qrc)

    file(WRITE ${resource} "<!DOCTYPE RCC><RCC version=\"1.0\">\n <qresource prefix=\"/\">\n")
    foreach(LANG ${qm_files})
        get_filename_component(file_name ${LANG} NAME)
        file(APPEND ${resource} "  <file>${file_name}</file>\n")
    endforeach()
    file(APPEND ${resource} " </qresource>\n</RCC>\n")

    set(${out_file} ${resource} PARENT_SCOPE)
endfunction()

function(add_translation out_files)
    set(qm_files "")
    foreach(filename ${ARGN})
        get_filename_component(basename ${filename} NAME_WE)
        set(qm "${CMAKE_CURRENT_SOURCE_DIR}/${basename}.qm")
        add_custom_command(
            OUTPUT "${qm}"
            COMMAND "${Qt5_LRELEASE_EXECUTABLE}"
            ARGS -markuntranslated "Is not translated!" -nounfinished -removeidentical -compress "${filename}" -qm "${qm}"
            DEPENDS "${filename}" VERBATIM
        )
        list(APPEND qm_files "${qm}")
    endforeach()

    set(${out_files} ${qm_files} PARENT_SCOPE)
endfunction()
