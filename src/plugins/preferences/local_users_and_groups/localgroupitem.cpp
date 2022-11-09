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

#include "localgroupitem.h"

#include "groupmemberscontaineritem.h"

namespace preferences
{

LocalGroupItem::LocalGroupItem()
    : BaseLocalItem("LocalGroupItem")
{
    addProperty(ACTION, 0);
    addProperty(GROUP_NAME, "");
    addProperty(GROUP_SID, "");
    addProperty(NEW_NAME, "");
    addProperty(DESCRIPTION, "");
    addProperty(USER_ACTION, "");
    addProperty(REMOVE_ACCOUNTS, false);
    addProperty(DELETE_ALL_USERS, false);
    addProperty(DELETE_ALL_GROUPS, false);

    addProperty<GroupMembersContainerItem>(MEMBERS)->setVisible(false);
}

LocalGroupItem::LocalGroupItem(const LocalGroupItem &other)
    : BaseLocalItem("LocalGroupItem")
{
    addProperty(ACTION, other.property<int>(ACTION));
    addProperty(GROUP_NAME, other.property<std::string>(GROUP_NAME));
    addProperty(GROUP_SID, other.property<std::string>(GROUP_SID));
    addProperty(NEW_NAME, other.property<std::string>(NEW_NAME));
    addProperty(DESCRIPTION, other.property<std::string>(DESCRIPTION));
    addProperty(USER_ACTION, other.property<std::string>(USER_ACTION));
    addProperty(REMOVE_ACCOUNTS, other.property<bool>(REMOVE_ACCOUNTS));
    addProperty(DELETE_ALL_USERS, other.property<bool>(DELETE_ALL_USERS));
    addProperty(DELETE_ALL_GROUPS, other.property<bool>(DELETE_ALL_GROUPS));
    addProperty(MEMBERS, other.property<GroupMembersContainerItem>(MEMBERS));
}

GroupMembersContainerItem *LocalGroupItem::getMembers() const
{
    return dynamic_cast<GroupMembersContainerItem*>(children().back());
}

}
