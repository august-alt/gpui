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

#include "defaultactions.h"

namespace preferences
{
int BaseModelBuilder::getActionCheckboxState(const std::string &data)
{
    if (data.compare("U") == 0)
    {
        return static_cast<int>(DefaultActions::UPDATE__MODE);
    }

    if (data.compare("R") == 0)
    {
        return static_cast<int>(DefaultActions::REPLACE_MODE);
    }

    if (data.compare("D") == 0)
    {
        return static_cast<int>(DefaultActions::DELETE__MODE);
    }

    return static_cast<int>(DefaultActions::CREATE__MODE);
}

std::string BaseModelBuilder::getActionCheckboxModel(const int &data)
{
    if (data == DefaultActions::UPDATE__MODE)
    {
        return "U";
    }

    if (data == DefaultActions::REPLACE_MODE)
    {
        return "R";
    }

    if (data == DefaultActions::DELETE__MODE)
    {
        return "D";
    }

    return "C";
}

} // namespace preferences
