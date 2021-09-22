find_package(Qt5 COMPONENTS Core Gui Widgets Test REQUIRED)

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

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(shortcutswidgettest ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES})
target_link_libraries(shortcutswidgettest ${GPUI_LIBRARIES} shortcuts-plugin Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test XercesC::XercesC)
add_gpui_test(plugins.shortcutswidgettest shortcutswidgettest)
