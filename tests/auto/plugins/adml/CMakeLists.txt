find_package(XercesC REQUIRED)

qt5_wrap_cpp(MOC_SOURCES admltest.h)
add_executable(admltest admltest.cpp ${MOC_SOURCES})
target_link_libraries(admltest ${GPUI_LIBRARIES} administrative-templates-plugin adml-plugin Qt5::Core Qt5::Test XercesC::XercesC)
add_gpui_test(plugins.admltest admltest)
