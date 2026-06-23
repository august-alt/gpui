find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Test REQUIRED)

find_package(XercesC REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    iniwidgettest.cpp
    ../../../../../src/plugins/preferences/ini/iniwidget.cpp
    ../../../../../src/plugins/preferences/ini/inischema.cpp
    ../../../../../src/plugins/preferences/ini/modelbuilder.cpp
)

set(UI_FORMS
    ../../../../../src/plugins/preferences/ini/iniwidget.ui
)

set(MOC_HEADERS
    iniwidgettest.h
    ../../../../../src/plugins/preferences/ini/iniwidget.h
)

qt_wrap_ui(UI_SOURCES ${UI_FORMS})
qt_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(iniwidgettest ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES})
target_link_libraries(iniwidgettest ${GPUI_LIBRARIES} ini-plugin Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test XercesC::XercesC)
add_gpui_test(plugins.iniwidgettest iniwidgettest)
