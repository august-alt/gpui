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

#include "drivesitem.h"

namespace preferences
{

DrivesItem::DrivesItem()
    : ModelView::CompoundItem("DrivesItem")
{
    addProperty(ACTION, "");
    addProperty(PATH, "");
    addProperty(PERSISTENT, false);
    addProperty(LABEL, "");
    addProperty(LETTER, "");
    addProperty(USER_NAME, "");
    addProperty(CPASSWORD, "");
    addProperty(USE_LETTER, true);
    addProperty(THIS_DRIVE, 0);
    addProperty(ALL_DRIVES, 0);
    addProperty(USE_EXISTING, !property<bool>(USE_LETTER));
}

DrivesItem::DrivesItem(const DrivesItem &other)
    : ModelView::CompoundItem("DrivesItem")
{
    addProperty(ACTION, other.property<std::string>(ACTION));
    addProperty(PATH, other.property<std::string>(PATH));
    addProperty(PERSISTENT, other.property<bool>(PERSISTENT));
    addProperty(LABEL, other.property<std::string>(LABEL));
    addProperty(LETTER, other.property<std::string>(LETTER));
    addProperty(USER_NAME, other.property<std::string>(USER_NAME));
    addProperty(CPASSWORD, other.property<std::string>(CPASSWORD));
    addProperty(USE_LETTER, other.property<bool>(USE_LETTER));
    addProperty(THIS_DRIVE, other.property<int>(THIS_DRIVE));
    addProperty(ALL_DRIVES, other.property<int>(ALL_DRIVES));
    addProperty(USE_EXISTING, other.property<bool>(USE_EXISTING));
}

}
