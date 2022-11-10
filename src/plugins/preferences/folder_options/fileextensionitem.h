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

#ifndef GPUI_FILEEXTENSION_ITEM_H
#define GPUI_FILEEXTENSION_ITEM_H

#include "common/basepreferenceitem.h"

namespace preferences
{

//! FileExtension item representation for editor.

class FileExtensionItem : public BasePreferenceItem<FileExtensionItem>
{
public:
    enum PropertyType
    {
        ACTION              = 0,
        FILE_EXT            = 1,
        APPLICATION         = 2,
        APP_PROG_ID         = 3,
        CONFIG_ACTIONS      = 4,
        ICON_PATH           = 5,
        ICON_INDEX          = 6,
        CONFIRM_OPEN        = 7,
        ALWAYS_SHOW         = 8,
        SAME_WINDOW         = 9,
        ACTIONS             = 10,
    };

public:
    FileExtensionItem();
    FileExtensionItem(const FileExtensionItem &other);

    constexpr static const char* propertyToString(const PropertyType& type);
};

}

Q_DECLARE_METATYPE(::preferences::FileExtensionItem)

#endif//GPUI_FILEEXTENSION_ITEM_H

