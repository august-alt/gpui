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

#include "basemodelbuilder.h"

namespace preferences
{
std::string BaseModelBuilder::getActionCheckboxState(const std::string &data)
{
    if (data.compare("U") == 0)
    {
        return "Update";
    }

    if (data.compare("R") == 0)
    {
        return "Replace";
    }

    if (data.compare("D") == 0)
    {
        return "Delete";
    }

    return "Create";
}

std::string BaseModelBuilder::getActionCheckboxModel(const std::string &data)
{
    if (data.compare("Update") == 0)
    {
        return "U";
    }

    if (data.compare("Replace") == 0)
    {
        return "R";
    }

    if (data.compare("Delete") == 0)
    {
        return "D";
    }

    return "C";
}

} // namespace preferences
