find_package(Qt5 COMPONENTS Core Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TEST_NAME smbfiletest)

set(SOURCES
    smbfiletest.cpp
    smbsharecreator.cpp
    ../../../../../src/plugins/storage/smb/smbclient.cpp
    ../../../../../src/plugins/storage/smb/smbdirectory.cpp
    ../../../../../src/plugins/storage/smb/smbfile.cpp
)

set(MOC_HEADERS
    smbfiletest.h
    smbsharecreator.h
)

qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

add_executable(${TEST_NAME} ${SOURCES} ${MOC_SOURCES})
target_link_libraries(${TEST_NAME} ${GPUI_LIBRARIES} Qt5::Core Qt5::Test)
add_gpui_test(plugins.${TEST_NAME} ${TEST_NAME})

## samba requires libsmbclient
find_path(SAMBA_INCLUDE_DIR
          NAMES libsmbclient.h
          HINTS /usr/include/smbclient
                /usr/include/samba
                /usr/include/samba-3.0
                /usr/include/samba-4.0
          )
find_library(SAMBA_LIBRARIES NAMES smbclient )
message(STATUS "samba include=${SAMBA_INCLUDE_DIR}")
message(STATUS "samba lib=${SAMBA_LIBRARIES}=${SAMBA_LIBRARIES}")

if(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
   message(STATUS "Found samba: include=${SAMBA_INCLUDE_DIR}  library=${SAMBA_LIBRARIES}")
   include_directories(${SAMBA_INCLUDE_DIR})
   target_link_libraries(${TEST_NAME} ${SAMBA_LIBRARIES})
else(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
   message(FATAL_ERROR "Could not find Samba libsmbclient")
endif(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
mark_as_advanced(SAMBA_INCLUDE_DIR SAMBA_LIBRARIES)
## end samba confiuration
