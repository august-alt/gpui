qt_wrap_cpp(MOC_SOURCES policybundletest.h)
add_executable(policybundletest policybundletest.cpp ${MOC_SOURCES})
target_link_libraries(policybundletest Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
add_gpui_test(model.policybundletest policybundletest)
