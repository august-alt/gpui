find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Test REQUIRED)

find_package(XercesC REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    driveswidgettest.cpp
    ../../../../../src/plugins/preferences/drives/driveswidget.cpp
    ../../../../../src/plugins/preferences/drives/drivesschema.cpp
    ../../../../../src/plugins/preferences/drives/modelbuilder.cpp
)

set(UI_FORMS
    ../../../../../src/plugins/preferences/drives/driveswidget.ui
)

set(MOC_HEADERS
    driveswidgettest.h
    ../../../../../src/plugins/preferences/drives/driveswidget.h
)

qt_wrap_ui(UI_SOURCES ${UI_FORMS})
qt_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(driveswidgettest ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES})
target_link_libraries(driveswidgettest ${GPUI_LIBRARIES} drives-plugin Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test XercesC::XercesC)
add_gpui_test(plugins.driveswidgettest driveswidgettest)
