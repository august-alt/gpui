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

#ifndef GPUI_REGISTRY_ENTRY_TYPE_H
#define GPUI_REGISTRY_ENTRY_TYPE_H

namespace model
{
namespace registry
{
enum RegistryEntryType
{
    REG_NONE      = 0x00,        // For initialization, you should never found this as a type.
    REG_SZ        = 0x01,        // Data is a null-terminated Unicode string.
    REG_EXPAND_SZ = 0x02,        // Data is a null-terminated Unicode string that contains operating system
                                 // environment variables denoted using an operating system-specific syntax.
    REG_BINARY           = 0x03, // Data is an octet stream.
    REG_DWORD            = 0x04, // Data is a 32-bit number in little-endian format.
    REG_DWORD_BIG_ENDIAN = 0x05, // Data is a 32-bit number in big-endian format.
    REG_MULTI_SZ         = 0x07, // Data is a sequence of characters terminated by two null Unicode characters,
                                 // and within that sequence zero or more null-terminated Unicode strings can exist.
    REG_QWORD = 0x0B,            // Data is a 64-bit number in little-endian format.
};
}
} // namespace model

#endif // GPUI_REGISTRY_ENTRY_TYPE_H
