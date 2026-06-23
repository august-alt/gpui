qt_wrap_cpp(MOC_SOURCES presentationbuildertest.h)

add_executable(presentationbuildertest presentationbuildertest.cpp ${MOC_SOURCES})
target_link_libraries(presentationbuildertest administrative-templates-plugin gpui-gui Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Test)
add_gpui_test(gui.presentationbuilder presentationbuildertest)
