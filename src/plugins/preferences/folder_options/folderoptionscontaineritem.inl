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

#include "common/commonitem.h"

#include <mvvm/signals/itemmapper.h>

namespace preferences
{

template <typename FolderOptionItem>
FolderOptionsContainerItem<FolderOptionItem>::FolderOptionsContainerItem()
    : ModelView::CompoundItem(typeid(FolderOptionsContainerItem<FolderOptionItem>).name())
{
    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<FolderOptionItem>(FOLDER_OPTION)->setVisible(false);
}

template <typename FolderOptionItem>
CommonItem FolderOptionsContainerItem<FolderOptionItem>::getCommon() const
{
    return property<CommonItem>(COMMON);
}

template <typename FolderOptionItem>
void FolderOptionsContainerItem<FolderOptionItem>::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

template <typename FolderOptionItem>
FolderOptionItem FolderOptionsContainerItem<FolderOptionItem>::getFolderOption() const
{
    return property<FolderOptionItem>(FOLDER_OPTION);
}

template <typename FolderOptionItem>
void FolderOptionsContainerItem<FolderOptionItem>::setFolderOption(const FolderOptionItem &item)
{
    setProperty(FOLDER_OPTION, item);
}

template <typename FolderOptionItem>
void FolderOptionsContainerItem<FolderOptionItem>::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem* item, std::string property)
    {
        if (auto folderOptionItem = dynamic_cast<FolderOptionItem*>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, folderOptionItem->template property<std::string>(ACTION));
            }

            if (property == NAME)
            {
                setProperty(NAME, folderOptionItem->template property<std::string>(NAME));
            }

            if (property == ASSOCIATION)
            {
                setProperty(ASSOCIATION, folderOptionItem->template property<std::string>(ASSOCIATION));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

}
