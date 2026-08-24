find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test REQUIRED)

qt_wrap_cpp(MOC_SOURCES basemodelbuildertest.h)

add_executable(basemodelbuildertest basemodelbuildertest.cpp ${MOC_SOURCES})
target_include_directories(basemodelbuildertest PRIVATE ${CMAKE_SOURCE_DIR}/src/plugins/preferences/common)
target_link_libraries(basemodelbuildertest preferences_static Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
add_gpui_test(plugins.basemodelbuildertest basemodelbuildertest)
