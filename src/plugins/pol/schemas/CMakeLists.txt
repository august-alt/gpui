set(RL_EXECUTABLE
    ragel
)

file(GLOB RL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.rl")

foreach(FILE ${RL_FILES})
    string(REGEX REPLACE "[.]rl" ".c" SOURCE ${FILE})

    add_custom_command(
        OUTPUT  "${SOURCE}"
        COMMAND ${RL_EXECUTABLE} ${RL_FLAGS} ${FILE}
        DEPENDS ${FILE}
        COMMENT "Running ${RL_EXECUTABLE} ${RL_FLAGS} ${FILE}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_EXPAND_LISTS
        VERBATIM
    )

    list(APPEND OUT_FILES "${SOURCE}")
endforeach()

set(RL_FLAGS
    -C
    -G2
)

set(POL_PARSER_LIBRARY_SOURCES
    parser.h
)

add_custom_target(rl_files ALL SOURCES ${RL_FILES})
add_custom_target(generate_all_files ALL DEPENDS ${OUT_FILES})

add_library(pol_parser STATIC ${POL_PARSER_LIBRARY_SOURCES} ${OUT_FILES})
set_property(TARGET pol_parser PROPERTY POSITION_INDEPENDENT_CODE ON)
add_dependencies(pol_parser generate_all_files)
