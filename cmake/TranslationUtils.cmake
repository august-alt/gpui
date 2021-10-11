function(add_translation_resource out_file)
    set(qm_files ${ARGN})
    set(resource ${CMAKE_CURRENT_SOURCE_DIR}/translations.qrc)

    file(WRITE ${resource} "<!DOCTYPE RCC><RCC version=\"1.0\">\n <qresource prefix=\"/\">\n")
    foreach(LANG ${qm_files})
        get_filename_component(file_name ${LANG} NAME)
        file(APPEND ${resource} "  <file>${file_name}</file>\n")
    endforeach()
    file(APPEND ${resource} " </qresource>\n</RCC>\n")

    set(${out_file} ${resource} PARENT_SCOPE)
endfunction()
