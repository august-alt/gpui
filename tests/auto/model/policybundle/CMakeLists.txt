qt5_wrap_cpp(MOC_SOURCES policybundletest.h)
add_executable(policybundletest policybundletest.cpp ${MOC_SOURCES})
target_link_libraries(policybundletest Qt5::Core Qt5::Test)
add_gpui_test(model.policybundletest policybundletest)
