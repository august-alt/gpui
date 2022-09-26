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

#ifndef GPUI_GROUPMEMBERSCONTAINER_ITEM_H
#define GPUI_GROUPMEMBERSCONTAINER_ITEM_H

#include <mvvm/model/groupitem.h>
#include <mvvm/model/compounditem.h>
#include <mvvm/model/sessionitemcontainer.h>
#include <mvvm/model/sessionitemtags.h>
#include <mvvm/model/taginfo.h>

namespace preferences
{

class GroupMemberItem;

class GroupMembersContainerItem : public ModelView::CompoundItem
{
public:
    static inline const std::string ITEMS = "ITEMS";
    static inline const std::string GROUP_ITEM_MEMBER_TYPE = "GroupMemberItem";

    GroupMembersContainerItem()
        : ModelView::CompoundItem("GroupMembersContainerItem")
    {
        registerTag(ModelView::TagInfo::universalTag(ITEMS, {GROUP_ITEM_MEMBER_TYPE}), true);
    }

    GroupMembersContainerItem(const GroupMembersContainerItem&)
        : ModelView::CompoundItem("GroupMembersContainerItem")
    {
        registerTag(ModelView::TagInfo::universalTag(ITEMS, {GROUP_ITEM_MEMBER_TYPE}), true);
    }
};

}

Q_DECLARE_METATYPE(::preferences::GroupMembersContainerItem)

#endif//GPUI_GROUPMEMBERSCONTAINER_ITEM_H
