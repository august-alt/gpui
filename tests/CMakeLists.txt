if (NOT ${GPUI_BUILD_TESTS})
  return()
endif()

macro(add_gpui_test test_name test_executable)
  add_test(NAME ${test_name} COMMAND ${test_executable})
endmacro(add_gpui_test)

add_subdirectory(auto)

# copy test data directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
