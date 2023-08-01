/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#include "sdmxformat.h"

namespace gpui
{

SdmxFormat::SdmxFormat()
    : io::PolicyFileFormat<io::PolicyResourcesFile>("sdmx")
{
}

bool SdmxFormat::read(std::istream &input, io::PolicyResourcesFile *file)
{
    Q_UNUSED(input);
    Q_UNUSED(file);

    return false;
}

bool SdmxFormat::write(std::ostream &output, io::PolicyResourcesFile *file)
{
    Q_UNUSED(output);
    Q_UNUSED(file);

    return false;
}

}
