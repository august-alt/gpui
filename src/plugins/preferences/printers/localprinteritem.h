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

#ifndef GPUI_LOCALPRINTER_ITEM_H
#define GPUI_LOCALPRINTER_ITEM_H

#include "common/basepreferenceitem.h"

namespace preferences
{

//! LocalPrinter item representation for editor.

class LocalPrinterItem : public BasePreferenceItem<LocalPrinterItem>
{
public:
    enum PropertyType
    {
        ACTION     = 0,
        NAME       = 1,
        PORT       = 2,
        PATH       = 3,
        DEFAULT    = 4,
        LOCATION   = 5,
        COMMENT    = 6,
        DELETE_ALL = 7
    };

public:
    LocalPrinterItem();
    LocalPrinterItem(const LocalPrinterItem &other);

    constexpr static inline const char* propertyToString(const PropertyType& type)
    {
        switch (type)
        {
        case ACTION:
            return "action";
        case NAME:
            return "name";
        case PORT:
            return "port";
        case PATH:
            return "path";
        case DEFAULT:
            return "default";
        case LOCATION:
            return "location";
        case COMMENT:
            return "comment";
        case DELETE_ALL:
            return "deleteAll";
        }
        return "";
    }
};

}

Q_DECLARE_METATYPE(::preferences::LocalPrinterItem)

#endif//GPUI_LOCALPRINTER_ITEM_H

