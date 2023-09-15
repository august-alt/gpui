find_package(GTest)
find_package(Qt5 COMPONENTS Core Gui Test REQUIRED)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

add_executable(settings-test examplesettingsobject.h settings-test.cpp)

#target_compile_options(settings-test PRIVATE -fsanitize=address)
#target_link_options(settings-test PRIVATE -fsanitize=address)

target_link_libraries(settings-test gpui-core Qt5::Core Qt5::Test Qt5::Gui gtest gtest_main)

add_gpui_test(core.settings-test settings-test)
