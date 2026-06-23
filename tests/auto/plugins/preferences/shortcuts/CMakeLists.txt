find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Test REQUIRED)

find_package(XercesC REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    shortcutswidgettest.cpp
    ../../../../../src/plugins/preferences/shortcuts/shortcutswidget.cpp
    ../../../../../src/plugins/preferences/shortcuts/shortcutsschema.cpp
    ../../../../../src/plugins/preferences/shortcuts/modelbuilder.cpp
)

set(UI_FORMS
    ../../../../../src/plugins/preferences/shortcuts/shortcutswidget.ui
)

set(MOC_HEADERS
    shortcutswidgettest.h
    ../../../../../src/plugins/preferences/shortcuts/shortcutswidget.h
)

qt_wrap_ui(UI_SOURCES ${UI_FORMS})
qt_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(shortcutswidgettest ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES})
target_link_libraries(shortcutswidgettest ${GPUI_LIBRARIES} shortcuts-plugin Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test XercesC::XercesC)
add_gpui_test(plugins.shortcutswidgettest shortcutswidgettest)
