qt5_wrap_cpp(MOC_SOURCES policyfiletest.h)
add_executable(policyfiletest policyfiletest.cpp ${MOC_SOURCES})
target_link_libraries(policyfiletest gpui-model gpui-io Qt5::Core Qt5::Test)
add_gpui_test(io.policyfiletest policyfiletest)
