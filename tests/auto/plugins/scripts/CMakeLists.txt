find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Test REQUIRED)
find_package(MVVM REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(scriptspolicyversiontest scriptspolicyversiontest.cpp)
target_include_directories(scriptspolicyversiontest PRIVATE
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/plugins/scripts)
target_link_libraries(scriptspolicyversiontest PRIVATE
    scripts-plugin gpui-gui gpui-core gpui-io
    ${MVVM_LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)
target_compile_definitions(scriptspolicyversiontest PRIVATE
    TEST_PLUGIN_DIR="${GPUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/gpui/plugins")
add_dependencies(scriptspolicyversiontest inifile-plugin inifile-ascii-plugin)
add_gpui_test(plugins.scripts.policyversion scriptspolicyversiontest)
