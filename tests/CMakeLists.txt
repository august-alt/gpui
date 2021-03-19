if (NOT ${GPUI_BUILD_TESTS})
  return()
endif()

macro(add_gpui_test test_name test_executable)
  add_test(NAME ${test_name} COMMAND ${test_executable})
endmacro(add_gpui_test)

add_subdirectory(auto)
