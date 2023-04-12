qt5_wrap_cpp(MOC_SOURCES templatefiltertest.h)
add_executable(templatefiltertest templatefiltertest.cpp ${MOC_SOURCES})
target_link_libraries(templatefiltertest administrative-templates-plugin gpui-gui Qt5::Core Qt5::Gui Qt5::Test)
add_gpui_test(gui.templatefiltertest templatefiltertest)
