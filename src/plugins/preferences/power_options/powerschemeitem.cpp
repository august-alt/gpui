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

#include "powerschemeitem.h"

namespace preferences
{

PowerSchemeItem::PowerSchemeItem()
    : BasePreferenceItem<PowerSchemeItem>("PowerSchemeItem")
{
    addProperty(propertyToString(ACTION), "");
    addProperty(propertyToString(NAME), "");
    addProperty(propertyToString(DEFAULT), false);
    addProperty(propertyToString(MONITOR_AC), "");
    addProperty(propertyToString(HARD_DISK_AC), "");
    addProperty(propertyToString(STANDBY_AC), "");
    addProperty(propertyToString(HIBERNATE_AC), "");
    addProperty(propertyToString(MONITOR_DC), "");
    addProperty(propertyToString(HARD_DISK_DC), "");
    addProperty(propertyToString(STANDBY_DC), "");
    addProperty(propertyToString(HIBERNATE_DC), "");
}

PowerSchemeItem::PowerSchemeItem(const PowerSchemeItem &other)
    : BasePreferenceItem<PowerSchemeItem>("PowerSchemeItem")
{
    copyProperty<std::string>(ACTION, other);
    copyProperty<std::string>(NAME, other);
    copyProperty<bool>(DEFAULT, other);
    copyProperty<std::string>(MONITOR_AC, other);
    copyProperty<std::string>(HARD_DISK_AC, other);
    copyProperty<std::string>(STANDBY_AC, other);
    copyProperty<std::string>(HIBERNATE_AC, other);
    copyProperty<std::string>(MONITOR_DC, other);
    copyProperty<std::string>(HARD_DISK_DC, other);
    copyProperty<std::string>(STANDBY_DC, other);
    copyProperty<std::string>(HIBERNATE_DC, other);
}

}
