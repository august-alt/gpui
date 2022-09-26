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

#ifndef GPUI_FOLDER_OPTIONS_CONTAINER_ITEM_H
#define GPUI_FOLDER_OPTIONS_CONTAINER_ITEM_H

#include <mvvm/model/compounditem.h>

#include "interfaces/containeriteminterface.h"

namespace preferences
{

//! Network container item representation for editor.

class CommonItem;
class BaseNetworkItem;
class DialUpItem;

template <typename FolderOptionItem>
class FolderOptionsContainerItem : public ModelView::CompoundItem, public ContainerItemInterface
{
public:
    static inline const std::string NAME = "name";
    static inline const std::string ORDER = "order";
    static inline const std::string ACTION = "action";
    static inline const std::string ASSOCIATION = "association";

    static inline const std::string COMMON        = "common";
    static inline const std::string FOLDER_OPTION = "network";

    explicit FolderOptionsContainerItem();

    CommonItem getCommon() const;
    void setCommon(const CommonItem& item);

    FolderOptionItem getFolderOption() const;
    void setFolderOption(const FolderOptionItem &item);

    void setupListeners() override;
};

}

#include "folderoptionscontaineritem.inl"

#endif//GPUI_FOLDER_OPTIONS_CONTAINER_ITEM_H
