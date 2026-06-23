find_package(XercesC REQUIRED)

qt_wrap_cpp(MOC_SOURCES admxtest.h)
add_executable(admxtest admxtest.cpp ${MOC_SOURCES})
target_link_libraries(admxtest ${GPUI_LIBRARIES} admx-plugin Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test XercesC::XercesC)
add_gpui_test(plugins.admxtest admxtest)
