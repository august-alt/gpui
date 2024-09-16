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

#ifndef GPUI_ADMX_FORMAT_H
#define GPUI_ADMX_FORMAT_H

#include "../../../src/core/common.h"

#include "../../../src/io/inifile.h"
#include "../../../src/io/policyfileformat.h"

namespace gpui
{
class GPUI_SYMBOL_EXPORT IniFormatAscii : public io::PolicyFileFormat<io::IniFile>
{
public:
    IniFormatAscii();

    bool read(std::istream &input, io::IniFile *file) override;

    bool write(std::ostream &output, io::IniFile *file) override;
};
} // namespace gpui

#include "iniformat.inl"

#endif // GPUI_ADMX_FORMAT_H
