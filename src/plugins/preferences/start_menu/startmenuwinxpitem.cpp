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

#include "startmenuwinxpitem.h"

namespace preferences
{

StartMenuWinXPItem::StartMenuWinXPItem()
    : BasePreferenceItem<::preferences::StartMenuWinXPItem>("StartMenuWinXPItem")
{
    addProperty(propertyToString(LARGE_MFU_ICONS), false);
    addProperty(propertyToString(MIN_MFU), 0);
    addProperty(propertyToString(AUTO_CASCADE), false);
    addProperty(propertyToString(NOTIFY_NEW_APPS), false);
    addProperty(propertyToString(SHOW_CONTROL_PANEL), "");
    addProperty(propertyToString(ENABLE_DRAG_DROP), false);
    addProperty(propertyToString(START_MENU_FAVORITES), "");
    addProperty(propertyToString(SHOW_HELP), false);
    addProperty(propertyToString(SHOW_MY_COMPUTER), "");
    addProperty(propertyToString(SHOW_MY_DOCS), "");
    addProperty(propertyToString(SHOW_MY_MUSIC), "");
    addProperty(propertyToString(SHOW_NET_PLACES ), false);
    addProperty(propertyToString(SHOW_MY_PICS), "");
    addProperty(propertyToString(SHOW_NET_CONN), "");
    addProperty(propertyToString(SHOW_PRINTERS), false);
    addProperty(propertyToString(SHOW_RUN), false);
    addProperty(propertyToString(SCROLL_PROGRAMS), false);
    addProperty(propertyToString(SHOW_SEARCH), false);
    addProperty(propertyToString(SHOW_RECENT_DOCS), "");
    addProperty(propertyToString(CLEAR_START_DOCS_LIST), false);
    addProperty(propertyToString(C_SHOW_LOGOFF), false);
    addProperty(propertyToString(C_SHOW_RUN), false);
    addProperty(propertyToString(C_ENABLE_DRAG_DROP), false);
    addProperty(propertyToString(C_CASCADE_CONTROL_PANEL), false);
    addProperty(propertyToString(C_CASCADE_MY_DOCUMENTS), false);
    addProperty(propertyToString(C_CASCADE_MY_PICTURES), false);
    addProperty(propertyToString(C_CASCADE_NETWORK_CONNECTIONS), false);
    addProperty(propertyToString(C_CASCADE_PRINTERS), false);
    addProperty(propertyToString(C_SCROLL_PROGRAMS), false);
    addProperty(propertyToString(C_PERSONALIZED), false);
}

StartMenuWinXPItem::StartMenuWinXPItem(const StartMenuWinXPItem &other)
    : BasePreferenceItem<::preferences::StartMenuWinXPItem>("StartMenuWinXPItem")
{
    copyProperty<bool>(LARGE_MFU_ICONS, other);
    copyProperty<int>(MIN_MFU, other);
    copyProperty<bool>(AUTO_CASCADE, other);
    copyProperty<bool>(NOTIFY_NEW_APPS, other);
    copyProperty<std::string>(SHOW_CONTROL_PANEL, other);
    copyProperty<bool>(ENABLE_DRAG_DROP, other);
    copyProperty<std::string>(START_MENU_FAVORITES, other);
    copyProperty<bool>(SHOW_HELP, other);
    copyProperty<std::string>(SHOW_MY_COMPUTER, other);
    copyProperty<std::string>(SHOW_MY_DOCS, other);
    copyProperty<std::string>(SHOW_MY_MUSIC, other);
    copyProperty<bool>(SHOW_NET_PLACES , other);
    copyProperty<std::string>(SHOW_MY_PICS, other);
    copyProperty<std::string>(SHOW_NET_CONN, other);
    copyProperty<bool>(SHOW_PRINTERS, other);
    copyProperty<bool>(SHOW_RUN, other);
    copyProperty<bool>(SCROLL_PROGRAMS, other);
    copyProperty<bool>(SHOW_SEARCH, other);
    copyProperty<std::string>(SHOW_RECENT_DOCS, other);
    copyProperty<bool>(CLEAR_START_DOCS_LIST, other);
    copyProperty<bool>(C_SHOW_LOGOFF, other);
    copyProperty<bool>(C_SHOW_RUN, other);
    copyProperty<bool>(C_ENABLE_DRAG_DROP, other);
    copyProperty<bool>(C_CASCADE_CONTROL_PANEL, other);
    copyProperty<bool>(C_CASCADE_MY_DOCUMENTS, other);
    copyProperty<bool>(C_CASCADE_MY_PICTURES, other);
    copyProperty<bool>(C_CASCADE_NETWORK_CONNECTIONS, other);
    copyProperty<bool>(C_CASCADE_PRINTERS, other);
    copyProperty<bool>(C_SCROLL_PROGRAMS, other);
    copyProperty<bool>(C_PERSONALIZED, other);
}

}
