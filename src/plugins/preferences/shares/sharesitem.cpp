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

#include "sharesitem.h"

namespace preferences
{
SharesItem::SharesItem()
    : ModelView::CompoundItem("SharesItem")
{
    addProperty(ACTION, 0);
    addProperty(NAME, "");
    addProperty(PATH, "");
    addProperty(COMMENT, "");
    addProperty(ALL_REGULAR, false);
    addProperty(ALL_HIDDEN, false);
    addProperty(ALL_ADMIN_DRIVE, false);
    addProperty(LIMIT_USERS, "NO_CHANGE");
    addProperty(USER_LIMIT, 0);
    addProperty(ACCESS_BASED_ENUMERATION, "NO_CHANGE");
}

SharesItem::SharesItem(const SharesItem &other)
    : ModelView::CompoundItem("SharesItem")
{
    addProperty(ACTION, other.property<int>(ACTION));
    addProperty(NAME, other.property<QString>(NAME));
    addProperty(PATH, other.property<QString>(PATH));
    addProperty(COMMENT, other.property<QString>(COMMENT));
    addProperty(ALL_REGULAR, other.property<bool>(ALL_REGULAR));
    addProperty(ALL_HIDDEN, other.property<bool>(ALL_HIDDEN));
    addProperty(ALL_ADMIN_DRIVE, other.property<bool>(ALL_ADMIN_DRIVE));
    addProperty(LIMIT_USERS, other.property<QString>(LIMIT_USERS));
    addProperty(USER_LIMIT, other.property<int>(USER_LIMIT));
    addProperty(ACCESS_BASED_ENUMERATION, other.property<QString>(ACCESS_BASED_ENUMERATION));
}

} // namespace preferences
