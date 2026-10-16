find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(gpoextensionstest gpoextensionstest.cpp)
target_include_directories(gpoextensionstest PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(gpoextensionstest PRIVATE gpui-ldap Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
add_gpui_test(ldap.gpoextensions gpoextensionstest)
