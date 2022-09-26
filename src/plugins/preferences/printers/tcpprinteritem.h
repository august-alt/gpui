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

#ifndef GPUI_TCP_PRINTER_ITEM_H
#define GPUI_TCP_PRINTER_ITEM_H

#include "common/basepreferenceitem.h"

namespace preferences
{

//! Drives item representation for editor.

class TcpPrinterItem : public BasePreferenceItem<TcpPrinterItem>
{
public:
    enum PropertyType
    {
        ACTION         = 0,
        IP_ADDRESS     = 1,
        USE_DNS        = 2,
        LOCAL_NAME     = 3,
        PATH           = 4,
        DEFAULT        = 5,
        SKIP_LOCAL     = 6,
        DELETE_ALL     = 7,
        LOCATION       = 8,
        COMMENT        = 9,
        LPR_QUEUE      = 10,
        SNMP_COMMUNITY = 11,
        TCP_PROTOCOL   = 12,
        PORT_NUMBER    = 13,
        DOUBLE_SPOOL   = 14,
        SNMP_ENABLED   = 15,
        SNMP_DEVINDEX  = 16,
        LPR_PROTOCOL   = 17
    };

public:
    TcpPrinterItem();
    TcpPrinterItem(const TcpPrinterItem &other);

    constexpr static inline const char *propertyToString(const PropertyType& type)
    {
        switch (type)
        {
        case ACTION:
            return("action");
        case IP_ADDRESS:
            return("ipAddress");
        case USE_DNS:
            return("useDNS");
        case LOCAL_NAME:
            return("localName");
        case PATH:
            return("path");
        case DEFAULT:
            return("default");
        case SKIP_LOCAL:
            return("skipLocal");
        case DELETE_ALL:
            return("deleteAll");
        case LOCATION:
            return("location");
        case COMMENT:
            return("comment");
        case LPR_QUEUE:
            return("lprQueue");
        case SNMP_COMMUNITY:
            return("snmpCommunity");
        case TCP_PROTOCOL:
            return("tcpProtocol");
        case PORT_NUMBER:
            return("portNumber");
        case DOUBLE_SPOOL:
            return("doubleSpool");
        case SNMP_ENABLED:
            return("snmpEnabled");
        case SNMP_DEVINDEX:
            return("snmpDevIndex");
        case LPR_PROTOCOL:
            return("lprProtocol");
        }

        return "";
    }
};

}

Q_DECLARE_METATYPE(::preferences::TcpPrinterItem)

#endif//GPUI_TCP_PRINTER_ITEM_H
