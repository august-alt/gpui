qt_wrap_cpp(MOC_SOURCES policyfiletest.h)
add_executable(policyfiletest policyfiletest.cpp ${MOC_SOURCES})
target_link_libraries(policyfiletest gpui-io Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
add_gpui_test(io.policyfiletest policyfiletest)
