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

#include "sharedprinteritem.h"

namespace preferences
{

SharedPrinterItem::SharedPrinterItem()
    : BasePreferenceItem<::preferences::SharedPrinterItem>("SharedPrinterItem")
{
    addProperty(propertyToString(ACTION), 0);
    addProperty(propertyToString(LOCATION), "");
    addProperty(propertyToString(PATH), "");
    addProperty(propertyToString(COMMENT), "");
    addProperty(propertyToString(DEFAULT), false);
    addProperty(propertyToString(SKIP_LOCAL), false);
    addProperty(propertyToString(DELETE_ALL), false);
    addProperty(propertyToString(PORT), "");
    addProperty(propertyToString(PERSISTENT), false);
    addProperty(propertyToString(DELETE_MAPS), false);
    addProperty(propertyToString(USERNAME), "");
    addProperty(propertyToString(PASSWORD), "");
}

SharedPrinterItem::SharedPrinterItem(const SharedPrinterItem &other)
    : BasePreferenceItem<::preferences::SharedPrinterItem>("SharedPrinterItem")
{
    copyProperty<int>(ACTION, other);
    copyProperty<std::string>(LOCATION, other);
    copyProperty<std::string>(PATH, other);
    copyProperty<std::string>(COMMENT, other);
    copyProperty<bool>(DEFAULT, other);
    copyProperty<bool>(SKIP_LOCAL, other);
    copyProperty<bool>(DELETE_ALL, other);
    copyProperty<std::string>(PORT, other);
    copyProperty<bool>(PERSISTENT, other);
    copyProperty<bool>(DELETE_MAPS, other);
    copyProperty<std::string>(USERNAME, other);
    copyProperty<std::string>(PASSWORD, other);
}

}
