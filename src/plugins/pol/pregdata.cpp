/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include "pregdata.h"

namespace preg {

std::string regtype2str(uint32_t &regtype) {
    std::string result = "UNKNOWN";

    switch (regtype) {
        case REG_NONE: {
            result = "REG_NONE";
            break;
        }
        case REG_SZ: {
            result = "REG_SZ";
            break;
        }
        case REG_EXPAND_SZ: {
            result = "REG_EXPAND_SZ";
            break;
        }
        case REG_BINARY: {
            result = "REG_BINARY";
            break;
        }
        case REG_DWORD_LITTLE_ENDIAN: {
            result = "REG_DWORD_LITTLE_ENDIAN";
            break;
        }
        case REG_DWORD_BIG_ENDIAN: {
            result = "REG_DWORD_BIG_ENDIAN";
            break;
        }
        case REG_LINK: {
            result = "REG_LINK";
            break;
        }
        case REG_MULTI_SZ: {
            result = "REG_MULTI_SZ";
            break;
        }
        case REG_RESOURCE_LIST: {
            result = "REG_RESOURCE_LIST";
            break;
        }
        case REG_FULL_RESOURCE_DESCRIPTOR: {
            result = "REG_FULL_RESOURCE_DESCRIPTOR";
            break;
        }
        case REG_RESOURCE_REQUIREMENTS_LIST: {
            result = "REG_RESOURCE_REQUIREMENTS_LIST";
            break;
        }
        case REG_QWORD: {
            result = "REG_QWORD";
            break;
        }
        case REG_QWORD_LITTLE_ENDIAN: {
            result = "REG_QWORD_LITTLE_ENDIAN";
            break;
        }
        default: {
            result = "UNKNOWN";
            break;
        }
    } /* switch (regtype) */

    return result;
} /* std::string regtype2str(uint16_t &regtype) */

uint32_t str2regtype(std::string &regtype) {
    uint32_t result = 0;

    if ("REG_NONE" == regtype) {
        result = REG_NONE;
    }
    if ("REG_SZ" == regtype) {
        result = REG_SZ;
    }
    if ("REG_EXPAND_SZ" == regtype) {
        result = REG_EXPAND_SZ;
    }
    if ("REG_BINARY" == regtype) {
        result = REG_BINARY;
    }
    if ("REG_DWORD_LITTLE_ENDIAN" == regtype || "REG_DWORD" == regtype) {
        result = REG_DWORD_LITTLE_ENDIAN;
    }
    if ("REG_DWORD_BIG_ENDIAN" == regtype) {
        result = REG_DWORD_BIG_ENDIAN;
    }
    if ("REG_LINK" == regtype) {
        result = REG_LINK;
    }
    if ("REG_MULTI_SZ" == regtype) {
        result = REG_MULTI_SZ;
    }
    if ("REG_RESOURCE_LIST" == regtype) {
        result = REG_RESOURCE_LIST;
    }
    if ("REG_FULL_RESOURCE_DESCRIPTOR" == regtype) {
        result = REG_FULL_RESOURCE_DESCRIPTOR;
    }
    if ("REG_RESOURCE_REQUIREMENTS_LIST" == regtype) {
        result = REG_RESOURCE_REQUIREMENTS_LIST;
    }
    if ("REG_QWORD" == regtype) {
        result = REG_QWORD;
    }
    if ("REG_QWORD_LITTLE_ENDIAN" == regtype) {
        result = REG_QWORD_LITTLE_ENDIAN;
    }

    return result;
} /* uint16_t str2regtype(std::string &regtype) */

const char *InvalidMagic::what() const throw() {
    return "Invalid PReg file magic value";
}

const char *InvalidVersion::what() const throw() {
    return "Invalid PReg file version";
}

}
