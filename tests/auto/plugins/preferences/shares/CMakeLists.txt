find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Test REQUIRED)

find_package(XercesC REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    shareswidgettest.cpp
    ../../../../../src/plugins/preferences/shares/shareswidget.cpp
    ../../../../../src/plugins/preferences/shares/sharesschema.cpp
    ../../../../../src/plugins/preferences/shares/modelbuilder.cpp
)

set(UI_FORMS
    ../../../../../src/plugins/preferences/shares/shareswidget.ui
)

set(MOC_HEADERS
    shareswidgettest.h
    ../../../../../src/plugins/preferences/shares/shareswidget.h
)

qt_wrap_ui(UI_SOURCES ${UI_FORMS})
qt_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(shareswidgettest ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES})
target_link_libraries(shareswidgettest ${GPUI_LIBRARIES} shares-plugin Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test XercesC::XercesC)
add_gpui_test(plugins.shareswidgettest shareswidgettest)
