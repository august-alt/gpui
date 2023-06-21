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

#ifndef GPUI_PREG_DATA_H
#define GPUI_PREG_DATA_H

#include <cstddef>
#include <exception>
#include <string>
#include <cstdint>

namespace preg {

/* Same as REG_BINARY */
const uint32_t REG_NONE = 0;

/* Null-terminated-string */
const uint32_t REG_SZ = 1;

/* A null-terminated UTF16-LE or ANSI string that contains unexpanded
 * references to environment variables.  */
const uint32_t REG_EXPAND_SZ = 2;

/* Any kind of binary data */
const uint32_t REG_BINARY = 3;

/* 32-bit number */
const uint32_t REG_DWORD_LITTLE_ENDIAN = 4;

/* 32-bit number in NBO format */
const uint32_t REG_DWORD_BIG_ENDIAN = 5;

/* A null-terminated Unicode string that contains the target path of a
 * symbolic link. */
const uint32_t REG_LINK = 6;

/* Sequence of null-terminated strings terminated by null-terminator */
const uint32_t REG_MULTI_SZ = 7;
const uint32_t REG_RESOURCE_LIST = 8;
const uint32_t REG_FULL_RESOURCE_DESCRIPTOR = 9;
const uint32_t REG_RESOURCE_REQUIREMENTS_LIST = 10;

/* 64-bit number */
const uint32_t REG_QWORD = 11;
const uint32_t REG_QWORD_LITTLE_ENDIAN = 12;

struct PregEntry
{
    std::string key {};
    std::string value {};
    uint32_t type = REG_NONE;
    uint32_t size = 0;
    char *data = nullptr;
};

class InvalidMagic : public std::exception {
public:
    virtual const char *what() const throw();
};

class InvalidVersion : public std::exception {
public:
    virtual const char *what() const throw();
};

/*!
 * \brief Convert PReg type value from DWORD into string representation.
 * May be useful when operating on PReg files from GUI.
 */
std::string regtype2str(uint32_t &regtype);

/*!
 * \brief Convert PReg type string representation into DWORD.
 * May be useful when serializing data.
 */
uint32_t str2regtype(std::string &regtype);

}

#endif // GPUI_PREG_DATA_H
