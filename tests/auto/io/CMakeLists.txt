find_package(GPUI COMPONENTS io model REQUIRED)
include_directories(${GPUI_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Test REQUIRED)
set(QT_USE_QTTEST TRUE)

add_subdirectory(policyfile)
