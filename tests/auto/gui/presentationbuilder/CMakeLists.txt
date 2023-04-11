qt5_wrap_cpp(MOC_SOURCES presentationbuildertest.h)

add_executable(presentationbuildertest presentationbuildertest.cpp ${MOC_SOURCES})
target_link_libraries(presentationbuildertest administrative-templates-plugin gpui-gui Qt5::Core Qt5::Gui Qt5::Test)
add_gpui_test(gui.presentationbuilder presentationbuildertest)
