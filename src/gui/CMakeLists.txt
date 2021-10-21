find_package(Qt5 COMPONENTS Widgets Qml LinguistTools REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
        /usr/include/libnemofolderlistmodel/
        /usr/include/libnemofolderlistmodel/net
        /usr/include/libnemofolderlistmodel/smb
        /usr/include/libnemofolderlistmodel/disk
        /usr/include/libnemofolderlistmodel/trash
    )

set(HEADERS
    contentwidget.h
    commandlineoptions.h
    commandlineparser.h
    gui.h
    listboxdialog.h
    mainwindow.h
    mainwindowsettings.h
    presentationbuilder.h
    smbfilebrowser.h
)

set(SOURCES
    commandlineparser.cpp
    contentwidget.cpp
    gui.cpp
    listboxdialog.cpp
    mainwindow.cpp
    mainwindowsettings.cpp
    presentationbuilder.cpp
    smbfilebrowser.cpp
    smbfilebrowserslots.cpp
)

set(UI_FORMS
    contentwidget.ui
    listboxdialog.ui
    mainwindow.ui
    smbfilebrowser.ui    
)

set(MOC_HEADERS
    contentwidget.h
    listboxdialog.h
    mainwindow.h
    mainwindowsettings.h
    smbfilebrowser.h
)

set(TS_FILES
    i18n/gui_en.ts
    i18n/gui_ru.ts
)

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR})

qt5_add_translation(QM_FILES ${TS_FILES})

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_definitions(
    -DGPUI_GUI_LIBRARY
)

add_translation_resource(RESOURCES_SRC ${QM_FILES})

qt5_add_resources(LIB_RESOURCES ${RESOURCES_SRC})

add_gpui_library(gpui-gui ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES} ${LIB_RESOURCES})
target_link_libraries(gpui-gui Qt5::Widgets Qt5::Qml)
target_link_libraries(gpui-gui gpui-model gpui-io)

find_package(PkgConfig REQUIRED)

pkg_check_modules(Nemo REQUIRED IMPORTED_TARGET libnemofolderlistmodel)
target_link_libraries(gpui-gui PkgConfig::Nemo)

## samba requires libsmbclient
find_path(SAMBA_INCLUDE_DIR
          NAMES libsmbclient.h
          HINTS /usr/include/smbclient /usr/include/samba /usr/include/samba-3.0 /usr/include/samba-4.0
          )
find_library(SAMBA_LIBRARIES NAMES smbclient )
message(STATUS "samba include=${SAMBA_INCLUDE_DIR}")
message(STATUS "samba lib=${SAMBA_LIBRARIES}=${SAMBA_LIBRARIES}")

if(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
   message(STATUS "Found samba: include=${SAMBA_INCLUDE_DIR}  library=${SAMBA_LIBRARIES}")
   INCLUDE_DIRECTORIES(${SAMBA_INCLUDE_DIR})
   TARGET_LINK_LIBRARIES(gpui-gui ${SAMBA_LIBRARIES})
else(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
   message(FATAL_ERROR "Could not find Samba libsmbclient")
endif(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
mark_as_advanced(SAMBA_INCLUDE_DIR SAMBA_LIBRARIES)
## end samba confiuration
