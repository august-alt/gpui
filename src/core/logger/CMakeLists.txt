find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
    loggermanager.h
    loggermessage.h
    logger.h
    consolelogger.h
    filelogger.h
    sysloglogger.h
)

set(SOURCES
    loggermanager.cpp
    logger.cpp
    consolelogger.cpp
    filelogger.cpp
    sysloglogger.cpp
)

add_definitions(
    -DGPUI_CORE_LIBRARY
)

add_library(gpui-logger STATIC ${SOURCES} ${HEADERS})
set_property(TARGET gpui-logger PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(gpui-logger PUBLIC ${Qt${QT_VERSION_MAJOR}Gui_INCLUDE_DIRS})
target_link_libraries(gpui-logger Qt${QT_VERSION_MAJOR}::Core)
