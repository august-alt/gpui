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

#include "tcpprinteritem.h"

namespace preferences
{

TcpPrinterItem::TcpPrinterItem()
    : BasePreferenceItem<::preferences::TcpPrinterItem>("TcpPrinterItem")
{
    addProperty(propertyToString(ACTION), "");
    addProperty(propertyToString(IP_ADDRESS), "");
    addProperty(propertyToString(USE_DNS), false);
    addProperty(propertyToString(LOCAL_NAME), "");
    addProperty(propertyToString(PATH), "");
    addProperty(propertyToString(DEFAULT), false);
    addProperty(propertyToString(SKIP_LOCAL), false);
    addProperty(propertyToString(DELETE_ALL), false);
    addProperty(propertyToString(LOCATION), "");
    addProperty(propertyToString(COMMENT), "");
    addProperty(propertyToString(LPR_QUEUE), "");
    addProperty(propertyToString(SNMP_COMMUNITY), "public");
    addProperty(propertyToString(TCP_PROTOCOL), true);
    addProperty(propertyToString(PORT_NUMBER), 0);
    addProperty(propertyToString(DOUBLE_SPOOL), false);
    addProperty(propertyToString(SNMP_ENABLED), false);
    addProperty(propertyToString(SNMP_DEVINDEX), 1);
    addProperty(propertyToString(LPR_PROTOCOL), false);
}

TcpPrinterItem::TcpPrinterItem(const TcpPrinterItem &other)
    : BasePreferenceItem<::preferences::TcpPrinterItem>("TcpPrinterItem")
{
    copyProperty<std::string>(ACTION, other);
    copyProperty<std::string>(IP_ADDRESS, other);
    copyProperty<bool>(USE_DNS, other);
    copyProperty<std::string>(LOCAL_NAME, other);
    copyProperty<std::string>(PATH, other);
    copyProperty<bool>(DEFAULT, other);
    copyProperty<bool>(SKIP_LOCAL, other);
    copyProperty<bool>(DELETE_ALL, other);
    copyProperty<std::string>(LOCATION, other);
    copyProperty<std::string>(COMMENT, other);
    copyProperty<std::string>(LPR_QUEUE, other);
    copyProperty<std::string>(SNMP_COMMUNITY, other);
    copyProperty<bool>(TCP_PROTOCOL, other);
    copyProperty<int>(PORT_NUMBER, other);
    copyProperty<bool>(DOUBLE_SPOOL, other);
    copyProperty<bool>(SNMP_ENABLED, other);
    copyProperty<int>(SNMP_DEVINDEX, other);
    copyProperty<bool>(LPR_PROTOCOL, other);
}

}
