find_package(GTest)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Test LinguistTools REQUIRED)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/i18n/test_ru.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/i18n/test_en.ts
)

add_translation(QM_FILES "${TS_FILES}")

add_translation_resource(RESOURCES_SRC translatorstorage-tests "${QM_FILES}")

qt_add_resources(LIB_RESOURCES ${RESOURCES_SRC})

add_executable(translatorstorage-test
    translatorstorage-test.cpp

    translatorstorageshadow.h
    translatorstorageshadow.cpp

    ${LIB_RESOURCES}
)

target_link_libraries(translatorstorage-test gpui-core Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Test
    gtest gtest_main)


add_gpui_test(core.translatorstorage-test translatorstorage-test)
