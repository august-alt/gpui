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

#include "vpnitem.h"

namespace preferences
{

VpnItem::VpnItem()
    : BaseNetworkItem("VpnItem")
{
    addProperty(propertyToString(ACTION), 0);
    addProperty(propertyToString(USER), false);
    addProperty(propertyToString(NAME), "");
    addProperty(propertyToString(IP_ADDRESS), "");
    addProperty(propertyToString(USE_DNS), false);
    addProperty(propertyToString(DIAL_FIRST), "");
    addProperty(propertyToString(TRAY_ICON), false);
    addProperty(propertyToString(SHOW_PROGRESS), false);
    addProperty(propertyToString(SHOW_PASSWORD), false);
    addProperty(propertyToString(SHOW_DOMAIN), false);
    addProperty(propertyToString(REDIAL_COUNT), 0);
    addProperty(propertyToString(REDIAL_PAUSE), 0);
    addProperty(propertyToString(IDLE_DISCONNECT), false);
    addProperty(propertyToString(RECONNECT), false);
    addProperty(propertyToString(CUSTOM_SETTINGS), false);
    addProperty(propertyToString(SECURE_PASSWORD), false);
    addProperty(propertyToString(SECURE_DATA), false);
    addProperty(propertyToString(USE_LOGON), false);
    addProperty(propertyToString(VPN_STRATEGY), "");
    addProperty(propertyToString(ENCRYPTION_TYPE), "");
    addProperty(propertyToString(EAP), false);
    addProperty(propertyToString(PAP), false);
    addProperty(propertyToString(SPAP), false);
    addProperty(propertyToString(MS_CHAP), false);
    addProperty(propertyToString(OLD_MS_CHAP), false);
    addProperty(propertyToString(MS_CHAP_V2), false);
    addProperty(propertyToString(SYSTEM), false);
}

VpnItem::VpnItem(const VpnItem &other)
    : BaseNetworkItem("VpnItem")
{
    copyProperty<int>(ACTION, other);
    copyProperty<bool>(USER, other);
    copyProperty<std::string>(NAME, other);
    copyProperty<std::string>(IP_ADDRESS, other);
    copyProperty<bool>(USE_DNS, other);
    copyProperty<std::string>(DIAL_FIRST, other);
    copyProperty<bool>(TRAY_ICON, other);
    copyProperty<bool>(SHOW_PROGRESS, other);
    copyProperty<bool>(SHOW_PASSWORD, other);
    copyProperty<bool>(SHOW_DOMAIN, other);
    copyProperty<int>(REDIAL_COUNT, other);
    copyProperty<int>(REDIAL_PAUSE, other);
    copyProperty<bool>(IDLE_DISCONNECT, other);
    copyProperty<bool>(RECONNECT, other);
    copyProperty<bool>(CUSTOM_SETTINGS, other);
    copyProperty<bool>(SECURE_PASSWORD, other);
    copyProperty<bool>(SECURE_DATA, other);
    copyProperty<bool>(USE_LOGON, other);
    copyProperty<std::string>(VPN_STRATEGY, other);
    copyProperty<std::string>(ENCRYPTION_TYPE, other);
    copyProperty<bool>(EAP, other);
    copyProperty<bool>(PAP, other);
    copyProperty<bool>(SPAP, other);
    copyProperty<bool>(MS_CHAP, other);
    copyProperty<bool>(OLD_MS_CHAP, other);
    copyProperty<bool>(MS_CHAP_V2, other);
    copyProperty<bool>(SYSTEM, other);
}

}
