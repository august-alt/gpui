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

#include "localuseritem.h"

#include <QDate>

namespace preferences
{

LocalUserItem::LocalUserItem()
    : BaseLocalItem("LocalUserItem")
{
    addProperty(ACTION, "");
    addProperty(USER_NAME, "");
    addProperty(NEW_NAME, "");
    addProperty(FULL_NAME, "");
    addProperty(DESCRIPTION, "");
    addProperty(CPASSWORD, "");
    addProperty(CHANGE_LOGON, false);
    addProperty(ACCOUNT_DISABLED, false);
    addProperty(PASSWORD_NEVER_EXPIRES, false);
    addProperty(EXPIRES, QDate::currentDate());
    addProperty(NO_CHANGE, false);
    addProperty(ACCOUNT_NEVER_EXPIRES, true);
}

LocalUserItem::LocalUserItem(const LocalUserItem &other)
    : BaseLocalItem("LocalUserItem")
{
    addProperty(ACTION, other.property<std::string>(ACTION));
    addProperty(USER_NAME, other.property<std::string>(USER_NAME));
    addProperty(NEW_NAME, other.property<std::string>(NEW_NAME));
    addProperty(FULL_NAME, other.property<std::string>(FULL_NAME));
    addProperty(DESCRIPTION, other.property<std::string>(DESCRIPTION));
    addProperty(CPASSWORD, other.property<std::string>(CPASSWORD));
    addProperty(CHANGE_LOGON, other.property<bool>(CHANGE_LOGON));
    addProperty(ACCOUNT_DISABLED, other.property<bool>(ACCOUNT_DISABLED));
    addProperty(PASSWORD_NEVER_EXPIRES, other.property<bool>(PASSWORD_NEVER_EXPIRES));
    addProperty(EXPIRES, other.property<QDate>(EXPIRES));
    addProperty(NO_CHANGE, other.property<bool>(NO_CHANGE));
    addProperty(ACCOUNT_NEVER_EXPIRES, other.property<bool>(ACCOUNT_NEVER_EXPIRES));
}

}
