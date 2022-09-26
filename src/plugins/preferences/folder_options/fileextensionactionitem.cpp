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

#include "fileextensionactionitem.h"

namespace preferences
{

FileExtensionActionItem::FileExtensionActionItem()
    : BasePreferenceItem<::preferences::FileExtensionActionItem>("FileExtensionActionItem")
{
    addProperty(propertyToString(NAME), "");
    addProperty(propertyToString(APP_USED), "");
    addProperty(propertyToString(DEFAULT), false);
    addProperty(propertyToString(USE_DDE), false);
    addProperty(propertyToString(DDE_MESSAGE), "");
    addProperty(propertyToString(DDE_APPLICATION), "");
    addProperty(propertyToString(DDE_APP_NOT_RUNNING), "");
    addProperty(propertyToString(DDE_TOPIC), "");
}

FileExtensionActionItem::FileExtensionActionItem(const FileExtensionActionItem &other)
    : BasePreferenceItem<::preferences::FileExtensionActionItem>("FileExtensionActionItem")
{
    copyProperty<std::string>(NAME, other);
    copyProperty<std::string>(APP_USED, other);
    copyProperty<bool>(DEFAULT, other);
    copyProperty<bool>(USE_DDE, other);
    copyProperty<std::string>(DDE_MESSAGE, other);
    copyProperty<std::string>(DDE_APPLICATION, other);
    copyProperty<std::string>(DDE_APP_NOT_RUNNING, other);
    copyProperty<std::string>(DDE_TOPIC, other);
}

constexpr const char *FileExtensionActionItem::propertyToString(const FileExtensionActionItem::PropertyType &type)
{
    switch (type)
    {
    case NAME:
        return("name");
    case APP_USED:
        return("appUsed");
    case DEFAULT:
        return("default");
    case USE_DDE:
        return("useDDE");
    case DDE_MESSAGE:
        return("ddeMessage");
    case DDE_APPLICATION:
        return("ddeApplication");
    case DDE_APP_NOT_RUNNING:
        return("ddeAppNotRunning");
    case DDE_TOPIC:
        return("ddeTopic");
    }

    return "";
}

}
