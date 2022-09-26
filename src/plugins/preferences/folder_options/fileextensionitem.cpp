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

#include "fileextensionitem.h"

#include "fileextensioncontaineritem.h"

namespace preferences
{

FileExtensionItem::FileExtensionItem()
    : BasePreferenceItem<::preferences::FileExtensionItem>("FileExtensionItem")
{
    addProperty(propertyToString(ACTION), "");
    addProperty(propertyToString(FILE_EXT), "");
    addProperty(propertyToString(APPLICATION), "");
    addProperty(propertyToString(APP_PROG_ID), "");
    addProperty(propertyToString(CONFIG_ACTIONS), false);
    addProperty(propertyToString(ICON_PATH), "");
    addProperty(propertyToString(ICON_INDEX), "");
    addProperty(propertyToString(CONFIRM_OPEN), false);
    addProperty(propertyToString(ALWAYS_SHOW), false);
    addProperty(propertyToString(SAME_WINDOW), false);

    addProperty<FileExtensionContainerItem>(propertyToString(ACTIONS))->setVisible(false);
}

FileExtensionItem::FileExtensionItem(const FileExtensionItem &other)
    : BasePreferenceItem<::preferences::FileExtensionItem>("FileExtensionItem")
{
    copyProperty<std::string>(ACTION, other);
    copyProperty<std::string>(FILE_EXT, other);
    copyProperty<std::string>(APPLICATION, other);
    copyProperty<std::string>(APP_PROG_ID, other);
    copyProperty<bool>(CONFIG_ACTIONS, other);
    copyProperty<std::string>(ICON_PATH, other);
    copyProperty<std::string>(ICON_INDEX, other);
    copyProperty<bool>(CONFIRM_OPEN, other);
    copyProperty<bool>(ALWAYS_SHOW, other);
    copyProperty<bool>(SAME_WINDOW, other);

    copyProperty<FileExtensionContainerItem>(ACTIONS, other);
}

constexpr const char *FileExtensionItem::propertyToString(const FileExtensionItem::PropertyType &type)
{
    switch (type)
    {
    case ACTION:
        return("action");
    case FILE_EXT:
        return("fileExt");
    case APPLICATION:
        return("application");
    case APP_PROG_ID:
        return("appProgID");
    case CONFIG_ACTIONS:
        return("configActions");
    case ICON_PATH:
        return("iconPath");
    case ICON_INDEX:
        return("iconIndex");
    case CONFIRM_OPEN:
        return("confirmOpen");
    case ALWAYS_SHOW:
        return("alwaysShow");
    case SAME_WINDOW:
        return("sameWindow");
    case ACTIONS:
        return "actions";
    }

    return "";
}

}

