find_package(Qt5 COMPONENTS Core Gui Widgets Test REQUIRED)

find_package(XercesC REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    folderswidgettest.cpp
    ../../../../../src/plugins/preferences/folders/folderswidget.cpp
    ../../../../../src/plugins/preferences/folders/foldersschema.cpp
    ../../../../../src/plugins/preferences/folders/modelbuilder.cpp
)

set(UI_FORMS
    ../../../../../src/plugins/preferences/folders/folderswidget.ui
)

set(MOC_HEADERS
    folderswidgettest.h
    ../../../../../src/plugins/preferences/folders/folderswidget.h
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(folderswidgettest ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES})
target_link_libraries(folderswidgettest ${GPUI_LIBRARIES} folders-plugin Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test XercesC::XercesC)
add_gpui_test(plugins.folderswidgettest folderswidgettest)
