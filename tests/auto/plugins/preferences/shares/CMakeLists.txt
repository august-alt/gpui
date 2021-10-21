find_package(Qt5 COMPONENTS Core Gui Widgets Test REQUIRED)

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

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(shareswidgettest ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES})
target_link_libraries(shareswidgettest ${GPUI_LIBRARIES} shares-plugin Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test XercesC::XercesC)
add_gpui_test(plugins.shareswidgettest shareswidgettest)
