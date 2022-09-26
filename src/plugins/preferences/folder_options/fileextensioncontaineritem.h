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

#ifndef GPUI_FILE_EXTENSION_CONTAINER_ITEM_H
#define GPUI_FILE_EXTENSION_CONTAINER_ITEM_H

#include <mvvm/model/compounditem.h>

namespace preferences
{

class FileExtensionContainerItem : public ModelView::CompoundItem
{
public:
    static inline const std::string ITEMS = "ITEMS";
    static inline const std::string FILE_EXTENSION_ACTION_TYPE = "FileExtensionActionItem";

    FileExtensionContainerItem();
    FileExtensionContainerItem(const FileExtensionContainerItem&);
};

}

Q_DECLARE_METATYPE(::preferences::FileExtensionContainerItem)

#endif//GPUI_FILE_EXTENSION_CONTAINER_ITEM_H
