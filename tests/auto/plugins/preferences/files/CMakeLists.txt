find_package(Qt5 COMPONENTS Core Gui Widgets Test REQUIRED)

find_package(XercesC REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    fileswidgettest.cpp
    ../../../../../src/plugins/preferences/files/fileswidget.cpp
    ../../../../../src/plugins/preferences/files/filesschema.cpp
    ../../../../../src/plugins/preferences/files/modelbuilder.cpp
)

set(UI_FORMS
    ../../../../../src/plugins/preferences/files/fileswidget.ui
)

set(MOC_HEADERS
    fileswidgettest.h
    ../../../../../src/plugins/preferences/files/fileswidget.h
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(fileswidgettest ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES})
target_link_libraries(fileswidgettest ${GPUI_LIBRARIES} files-plugin Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test XercesC::XercesC)
add_gpui_test(plugins.fileswidgettest fileswidgettest)
