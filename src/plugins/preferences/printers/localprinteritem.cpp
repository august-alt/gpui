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

#include "localprinteritem.h"

namespace preferences
{

LocalPrinterItem::LocalPrinterItem()
    : BasePreferenceItem<::preferences::LocalPrinterItem>("LocalPrinterItem")
{
    addProperty(propertyToString(ACTION), "");
    addProperty(propertyToString(NAME), "");
    addProperty(propertyToString(PORT), "");
    addProperty(propertyToString(PATH), "");
    addProperty(propertyToString(DEFAULT), false);
    addProperty(propertyToString(LOCATION), "");
    addProperty(propertyToString(COMMENT), "");
    addProperty(propertyToString(DELETE_ALL), false);
}

LocalPrinterItem::LocalPrinterItem(const LocalPrinterItem &other)
    : BasePreferenceItem<::preferences::LocalPrinterItem>("LocalPrinterItem")
{
    copyProperty<std::string>(ACTION, other);
    copyProperty<std::string>(NAME, other);
    copyProperty<std::string>(PORT, other);
    copyProperty<std::string>(PATH, other);
    copyProperty<bool>(DEFAULT, other);
    copyProperty<std::string>(LOCATION, other);
    copyProperty<std::string>(COMMENT, other);
    copyProperty<bool>(DELETE_ALL, other);
}

}

