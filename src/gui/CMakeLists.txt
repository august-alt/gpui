find_package(Qt5 COMPONENTS Widgets Qml REQUIRED)

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
    gui.h
    mainwindow.h
    mainwindowsettings.h
    presentationbuilder.h
    smbfilebrowser.h
)

set(SOURCES
    contentwidget.cpp
    gui.cpp
    mainwindow.cpp
    mainwindowsettings.cpp
    presentationbuilder.cpp
    smbfilebrowser.cpp
    smbfilebrowserslots.cpp
)

set(UI_FORMS
    contentwidget.ui
    mainwindow.ui
    smbfilebrowser.ui
)

set(MOC_HEADERS
    contentwidget.h
    mainwindow.h
    mainwindowsettings.h
    smbfilebrowser.h
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_definitions(
    -DGPUI_GUI_LIBRARY
)

add_gpui_library(gpui-gui ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES})
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
