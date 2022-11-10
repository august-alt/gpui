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

#ifndef GPUI_LOCALUSER_ITEM_H
#define GPUI_LOCALUSER_ITEM_H

#include "baselocalitem.h"

namespace preferences
{

//! Drives item representation for editor.

class LocalUserItem : public BaseLocalItem
{
public:
    static inline const std::string ACTION = "action";
    static inline const std::string USER_NAME = "userName";
    static inline const std::string NEW_NAME = "newName";
    static inline const std::string FULL_NAME = "fullName";
    static inline const std::string DESCRIPTION = "description";
    static inline const std::string CPASSWORD = "cpassword";
    static inline const std::string CHANGE_LOGON = "changeLogon";
    static inline const std::string ACCOUNT_DISABLED = "acctDisabled";
    static inline const std::string PASSWORD_NEVER_EXPIRES = "neverExpires";
    static inline const std::string EXPIRES = "expires";
    static inline const std::string NO_CHANGE = "nochange";
    static inline const std::string ACCOUNT_NEVER_EXPIRES = "accountNeverExpires";

    LocalUserItem();
    LocalUserItem(const LocalUserItem &other);
};

}

Q_DECLARE_METATYPE(::preferences::LocalUserItem)

#endif//GPUI_LocalUser_ITEM_H
