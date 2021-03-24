find_package(XercesC REQUIRED)

qt5_wrap_cpp(MOC_SOURCES admxtest.h)
add_executable(admxtest admxtest.cpp ${MOC_SOURCES})
target_link_libraries(admxtest ${GPUI_LIBRARIES} admx-plugin Qt5::Core Qt5::Test XercesC::XercesC)
add_gpui_test(plugins.admxtest admxtest)
