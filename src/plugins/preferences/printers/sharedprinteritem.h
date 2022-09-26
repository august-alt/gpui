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

#ifndef GPUI_SHARED_PRINTER_ITEM_H
#define GPUI_SHARED_PRINTER_ITEM_H

#include "common/basepreferenceitem.h"

namespace preferences
{

//! Drives item representation for editor.

class SharedPrinterItem : public BasePreferenceItem<SharedPrinterItem>
{
public:
    enum PropertyType
    {
        ACTION      = 0,
        LOCATION    = 1,
        PATH        = 2,
        COMMENT     = 3,
        DEFAULT     = 4,
        SKIP_LOCAL  = 5,
        DELETE_ALL  = 6,
        PORT        = 7,
        PERSISTENT  = 8,
        DELETE_MAPS = 9,
        USERNAME    = 10,
        PASSWORD    = 11
    };

public:
    SharedPrinterItem();
    SharedPrinterItem(const SharedPrinterItem &other);

    constexpr static inline const char *propertyToString(const PropertyType& type)
    {
        switch (type)
        {
        case ACTION:
            return "action";
        case LOCATION:
            return "location";
        case PATH:
            return "path";
        case COMMENT:
            return "comment";
        case DEFAULT:
            return "default";
        case SKIP_LOCAL:
            return "skipLocal";
        case DELETE_ALL:
            return "deleteAll";
        case PORT:
            return "port";
        case PERSISTENT:
            return "persistent";
        case DELETE_MAPS:
            return "deleteMaps";
        case USERNAME:
            return "username";
        case PASSWORD:
            return "cpassword";
        }

        return "";
    }
};

}

Q_DECLARE_METATYPE(::preferences::SharedPrinterItem)

#endif//GPUI_SHARED_PRINTER_ITEM_H
