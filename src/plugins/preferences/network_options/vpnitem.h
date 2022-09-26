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

#ifndef GPUI_VPN_ITEM_H
#define GPUI_VPN_ITEM_H

#include "basenetworkitem.h"

namespace preferences
{

//! Drives item representation for editor.

class VpnItem : public BaseNetworkItem
{
public:
    enum PropertyType
    {
        ACTION          = 0,
        USER            = 1,
        NAME            = 2,
        IP_ADDRESS      = 3,
        USE_DNS         = 4,
        DIAL_FIRST      = 5,
        TRAY_ICON       = 6,
        SHOW_PROGRESS   = 7,
        SHOW_PASSWORD   = 8,
        SHOW_DOMAIN     = 9,
        REDIAL_COUNT    = 10,
        REDIAL_PAUSE    = 11,
        IDLE_DISCONNECT = 12,
        RECONNECT       = 13,
        CUSTOM_SETTINGS = 14,
        SECURE_PASSWORD = 15,
        SECURE_DATA     = 16,
        USE_LOGON       = 17,
        VPN_STRATEGY    = 18,
        ENCRYPTION_TYPE = 19,
        EAP             = 20,
        PAP             = 21,
        SPAP            = 22,
        CHAP            = 23,
        MS_CHAP         = 24,
        OLD_MS_CHAP     = 25,
        MS_CHAP_V2      = 26,
        SYSTEM          = 27,
        TYPICAL         = 28,
        ADVANCED        = 29,
        USE_OTHER       = 30
    };

public:
    VpnItem();
    VpnItem(const VpnItem &other);

    constexpr static int propertyToInt(const PropertyType& type);
    constexpr static const char* propertyToString(const PropertyType& type)
    {
        switch (type)
        {
        case ACTION:
            return "action";
        case USER:
            return "user";
        case NAME:
            return "name";
        case IP_ADDRESS:
            return "ipAddress";
        case USE_DNS:
            return "useDNS";
        case DIAL_FIRST:
            return "dialFirst";
        case TRAY_ICON:
            return "trayIcon";
        case SHOW_PROGRESS:
            return "showProgress";
        case SHOW_PASSWORD:
            return "showPassword";
        case SHOW_DOMAIN:
            return "showDomain";
        case REDIAL_COUNT:
            return "redialCount";
        case REDIAL_PAUSE:
            return "redialPause";
        case IDLE_DISCONNECT:
            return "idleDisconnect";
        case RECONNECT:
            return "reconnect";
        case CUSTOM_SETTINGS:
            return "customSettings";
        case SECURE_PASSWORD:
            return "securePassword";
        case SECURE_DATA:
            return "secureData";
        case USE_LOGON:
            return "useLogon";
        case VPN_STRATEGY:
            return "vpnStrategy";
        case ENCRYPTION_TYPE:
            return "encryptionType";
        case EAP:
            return "eap";
        case PAP:
            return "pap";
        case SPAP:
            return "spap";
        case CHAP:
            return "chap";
        case MS_CHAP:
            return "msChap";
        case OLD_MS_CHAP:
            return "oldMsChap";
        case MS_CHAP_V2:
            return "msChapV2";
        case SYSTEM:
            return "system";
        case TYPICAL:
            return "typical";
        case ADVANCED:
            return "advanced";
        case USE_OTHER:
            return "useOther";
        default:
            break;
        }

        return "";
    }

private:
    template <typename T>
    inline void copyProperty(const PropertyType& type, const VpnItem &other);
};

template<typename T>
void VpnItem::copyProperty(const VpnItem::PropertyType &type, const VpnItem &other)
{
    addProperty(propertyToString(type), other.property<T>(propertyToString(type)));
}

inline constexpr int VpnItem::propertyToInt(const VpnItem::PropertyType &type)
{
    return static_cast<int>(type);
}

}

Q_DECLARE_METATYPE(::preferences::VpnItem)

#endif//GPUI_Vpn_ITEM_H
