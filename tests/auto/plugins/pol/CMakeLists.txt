qt5_wrap_cpp(MOC_SOURCES poltest.h)
add_executable(poltest poltest.cpp ${MOC_SOURCES})
target_link_libraries(poltest ${GPUI_LIBRARIES} pol-plugin Qt5::Core Qt5::Test)
add_gpui_test(plugins.poltest poltest)
