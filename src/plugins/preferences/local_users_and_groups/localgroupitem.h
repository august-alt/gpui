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

#ifndef GPUI_LOCALGROUP_ITEM_H
#define GPUI_LOCALGROUP_ITEM_H

#include "baselocalitem.h"

namespace preferences
{

class GroupMembersContainerItem;

//! Drives item representation for editor.

class LocalGroupItem : public BaseLocalItem
{
public:
    static inline const std::string ACTION = "action";
    static inline const std::string GROUP_NAME = "groupName";
    static inline const std::string GROUP_SID = "groupSid";
    static inline const std::string NEW_NAME = "newName";
    static inline const std::string DESCRIPTION = "description";
    static inline const std::string USER_ACTION = "userAction";
    static inline const std::string REMOVE_ACCOUNTS = "removeAccounts";
    static inline const std::string DELETE_ALL_USERS = "deleteAllUsers";
    static inline const std::string DELETE_ALL_GROUPS = "deleteAllGroups";
    static inline const std::string MEMBERS = "Members";

    LocalGroupItem();
    LocalGroupItem(const LocalGroupItem &other);

    GroupMembersContainerItem* getMembers() const;
};

}

Q_DECLARE_METATYPE(::preferences::LocalGroupItem)

#endif//GPUI_LocalGroup_ITEM_H
