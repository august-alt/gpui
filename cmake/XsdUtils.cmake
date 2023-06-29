cmake_policy(SET CMP0071 NEW)

function(add_xsd_library library_name)
    set(XSD_FILES ${ARGN})

    foreach(FILE ${XSD_FILES})
        string(REGEX REPLACE "[.]xsd" ".h" HEADER ${FILE})
        set(XSD_HEADERS ${XSD_HEADERS} ${HEADER})
    endforeach()

    foreach(FILE ${XSD_FILES})
        string(REGEX REPLACE "[.]xsd" ".cpp" SOURCE ${FILE})
        set(XSD_SOURCES ${XSD_SOURCES} ${SOURCE})
    endforeach()

    set(OUT_FILES
        ${XSD_HEADERS}
        ${XSD_SOURCES}
    )

    set(XSD_FLAGS cxx-tree
        --generate-doxygen
        --generate-ostream
        --generate-serialization
        --generate-polymorphic
        --polymorphic-type-all
        --generate-wildcard
        --cxx-suffix .cpp
        --hxx-suffix .h
        --std c++11
        --type-naming ucc
        --root-element-all
    )

    add_custom_command(
        OUTPUT ${OUT_FILES}
        COMMAND ${XSDCXX_EXECUTABLE} ${XSD_FLAGS} ${XSD_FILES}
        DEPENDS ${XSD_FILES}
        COMMENT "Running ${XSDCXX_EXECUTABLE} ${XSD_FLAGS} ${XSD_FILES}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_EXPAND_LISTS
    )

    add_library(${library_name} STATIC ${OUT_FILES})
endfunction()
