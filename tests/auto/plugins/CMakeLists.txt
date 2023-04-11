find_package(GPUI COMPONENTS core io REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Test REQUIRED)
set(QT_USE_QTTEST TRUE)

add_subdirectory(adml)
add_subdirectory(admx)
add_subdirectory(pol)
add_subdirectory(preferences)
add_subdirectory(storage)
