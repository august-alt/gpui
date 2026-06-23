qt_wrap_cpp(MOC_SOURCES poltest.h)

set(CMAKE_CXX_STANDARD 17)

add_executable(poltest poltest.cpp ${MOC_SOURCES})
target_link_libraries(poltest ${GPUI_LIBRARIES} pol-plugin Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
add_gpui_test(plugins.poltest poltest)
