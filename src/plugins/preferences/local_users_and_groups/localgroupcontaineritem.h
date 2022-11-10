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

#ifndef GPUI_LOCALGROUP_CONTAINER_ITEM_H
#define GPUI_LOCALGROUP_CONTAINER_ITEM_H

#include "interfaces/containeriteminterface.h"

#include <mvvm/model/compounditem.h>

namespace preferences
{
//! Folder item representation for editor.

class CommonItem;
class LocalGroupItem;

template<typename LocalGroupItem>
class LocalGroupContainerItem : public ModelView::CompoundItem, public ContainerItemInterface
{
public:
    static inline const std::string NAME        = "name";
    static inline const std::string ORDER       = "order";
    static inline const std::string ACTION      = "action";
    static inline const std::string DESCRIPTION = "description";
    static inline const std::string FULL_NAME   = "fullName";
    static inline const std::string USER_NAME   = "userName";
    static inline const std::string GROUP_NAME  = "groupName";

    static inline const std::string COMMON              = "common";
    static inline const std::string LOCAL_USER_OR_GROUP = "localUserOrGroupGroup";

    explicit LocalGroupContainerItem();

    CommonItem *getCommon() const;
    void setCommon(const CommonItem &item);

    LocalGroupItem getLocalGroup() const;
    void setLocalGroup(const LocalGroupItem &item);

    void setupListeners() override;

    void retranslateStrings() override;
};

} // namespace preferences

#include "localgroupcontaineritem.inl"

#endif //GPUI_LOCALGROUP_CONTAINER_ITEM_H
