find_package(Qt5 COMPONENTS Core Gui REQUIRED)
find_package(Ldap REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NdrStandard REQUIRED IMPORTED_TARGET ndr_standard)
pkg_check_modules(Smbclient REQUIRED IMPORTED_TARGET smbclient)
pkg_check_modules(Krb5 REQUIRED IMPORTED_TARGET krb5)
pkg_check_modules(Glib REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(Uuid REQUIRED IMPORTED_TARGET uuid)
pkg_check_modules(Sasl REQUIRED IMPORTED_TARGET libsasl2)
pkg_check_modules(Ndr REQUIRED IMPORTED_TARGET ndr)

set(NDR_VERSION_GREATER_THAN_1_0_1 OFF)
if(Ndr_VERSION VERSION_GREATER "1.0.1")
    set(NDR_VERSION_GREATER_THAN_1_0_1 ON)
endif()
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/adldap_config.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/adldap_config.h"
    IMMEDIATE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(HEADERS
    ldapcontract.h
    ldapimpl.h
    ad_config.h
    ad_config_p.h
    ad_interface.h
    ad_interface_p.h
    ad_utils.h
    ad_defines.h
    ad_object.h
    ad_filter.h
    ad_security.h
    ad_display.h
    gplink.h
    adldap.h
    ldap-common.h

    samba/ndr_security.h
)

set(SOURCES
    ldapcontract.cpp
    ldapimpl.cpp
    ad_config.cpp
    ad_interface.cpp
    ad_utils.cpp
    ad_object.cpp
    ad_filter.cpp
    ad_security.cpp
    ad_display.cpp
    gplink.cpp

    samba/gp_manage.c
    samba/ndr_security.c
    samba/dom_sid.c
    samba/replace.c
    samba/ndr_misc.c
    samba/libsmb_xattr.c
    samba/security_descriptor.c
)

add_definitions(
    -DGPUI_LDAP_LIBRARY
)

add_gpui_library(gpui-ldap ${HEADERS} ${SOURCES})
target_link_libraries(gpui-ldap Qt5::Core
    PkgConfig::Uuid
    PkgConfig::Smbclient
    PkgConfig::Krb5
    Ldap::Ldap
    PkgConfig::NdrStandard
    PkgConfig::Sasl
    resolv
)
