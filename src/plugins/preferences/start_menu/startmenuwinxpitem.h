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

#ifndef GPUI_START_MENU_WIN_XP_ITEM_H
#define GPUI_START_MENU_WIN_XP_ITEM_H

#include "common/basepreferenceitem.h"

namespace preferences
{

//! Drives item representation for editor.

class StartMenuWinXPItem : public BasePreferenceItem<StartMenuWinXPItem>
{
public:
    enum PropertyType
    {
        LARGE_MFU_ICONS               = 0,
        MIN_MFU                       = 1,
        AUTO_CASCADE                  = 2,
        NOTIFY_NEW_APPS               = 3,
        SHOW_CONTROL_PANEL            = 4,
        ENABLE_DRAG_DROP              = 5,
        START_MENU_FAVORITES          = 6,
        SHOW_HELP                     = 7,
        SHOW_MY_COMPUTER              = 8,
        SHOW_MY_DOCS                  = 9,
        SHOW_MY_MUSIC                 = 10,
        SHOW_NET_PLACES               = 11,
        SHOW_MY_PICS                  = 12,
        SHOW_NET_CONN                 = 13,
        SHOW_PRINTERS                 = 14,
        SHOW_RUN                      = 15,
        SCROLL_PROGRAMS               = 16,
        SHOW_SEARCH                   = 17,
        SHOW_RECENT_DOCS              = 18,
        CLEAR_START_DOCS_LIST         = 19,
        C_SHOW_LOGOFF                 = 20,
        C_SHOW_RUN                    = 21,
        C_ENABLE_DRAG_DROP            = 22,
        C_CASCADE_CONTROL_PANEL       = 23,
        C_CASCADE_MY_DOCUMENTS        = 24,
        C_CASCADE_MY_PICTURES         = 25,
        C_CASCADE_NETWORK_CONNECTIONS = 26,
        C_CASCADE_PRINTERS            = 27,
        C_SCROLL_PROGRAMS             = 28,
        C_PERSONALIZED                = 29,
    };

public:
    StartMenuWinXPItem();
    StartMenuWinXPItem(const StartMenuWinXPItem &other);

    constexpr static inline const char* propertyToString(const PropertyType& type)
    {
        switch (type)
        {
        case LARGE_MFU_ICONS: return "largeMFUIcons";
        case MIN_MFU: return "minMFU";
        case AUTO_CASCADE: return "autoCascade";
        case NOTIFY_NEW_APPS: return "notifyNewApps";
        case SHOW_CONTROL_PANEL: return "showControlPanel";
        case ENABLE_DRAG_DROP: return "enableDragDrop";
        case START_MENU_FAVORITES: return "startMenuFavorites";
        case SHOW_HELP: return "showHelp";
        case SHOW_MY_COMPUTER: return "showMyComputer";
        case SHOW_MY_DOCS: return "showMyDocs";
        case SHOW_MY_MUSIC: return "showMyMusic";
        case SHOW_NET_PLACES: return "showNetPlaces";
        case SHOW_MY_PICS: return "showMyPics";
        case SHOW_NET_CONN: return "showNetConn";
        case SHOW_PRINTERS: return "showPrinters";
        case SHOW_RUN: return "showRun";
        case SCROLL_PROGRAMS: return "scrollPrograms";
        case SHOW_SEARCH: return "showSearch";
        case SHOW_RECENT_DOCS: return "showRecentDocs";
        case CLEAR_START_DOCS_LIST: return "clearStartDocsList";
        case C_SHOW_LOGOFF: return "cShowLogoff";
        case C_SHOW_RUN: return "cShowRun";
        case C_ENABLE_DRAG_DROP: return "cEnableDragDrop";
        case C_CASCADE_CONTROL_PANEL: return "cCascadeControlPanel";
        case C_CASCADE_MY_DOCUMENTS: return "cCascadeMyDocuments";
        case C_CASCADE_MY_PICTURES: return "cCascadeMyPictures";
        case C_CASCADE_NETWORK_CONNECTIONS: return "cCascadeNetworkConnections";
        case C_CASCADE_PRINTERS: return "cCascadePrinters";
        case C_SCROLL_PROGRAMS: return "cScrollPrograms";
        case C_PERSONALIZED: return "cPersonalized";
        }
        return "";
    }
};

}

Q_DECLARE_METATYPE(::preferences::StartMenuWinXPItem)

#endif // GPUI_START_MENU_WIN_XP_ITEM_H
