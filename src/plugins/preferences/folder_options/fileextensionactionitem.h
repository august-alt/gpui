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

#ifndef GPUI_FILE_EXTENSION_ACTION_ITEM_H
#define GPUI_FILE_EXTENSION_ACTION_ITEM_H

#include "common/basepreferenceitem.h"

namespace preferences
{

//! FileExtension item representation for editor.

class FileExtensionActionItem : public BasePreferenceItem<FileExtensionActionItem>
{
public:
    enum PropertyType
    {
        NAME                = 0,
        APP_USED            = 1,
        DEFAULT             = 2,
        USE_DDE             = 3,
        DDE_MESSAGE         = 4,
        DDE_APPLICATION     = 5,
        DDE_APP_NOT_RUNNING = 6,
        DDE_TOPIC           = 7,
    };

public:
    FileExtensionActionItem();
    FileExtensionActionItem(const FileExtensionActionItem &other);

    constexpr static const char* propertyToString(const PropertyType& type);
};

}

Q_DECLARE_METATYPE(::preferences::FileExtensionActionItem)

#endif // GPUI_FILE_EXTENSION_ACTION_ITEM_H
