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

namespace preferences
{

constexpr const char *PowerSchemeItem::propertyToString(const PowerSchemeItem::PropertyType &type)
{
    switch (type)
    {
    case ACTION:
        return "action";
    case NAME:
        return "name";
    case DEFAULT:
        return "default";
    case MONITOR_AC:
        return "monitorAc";
    case HARD_DISK_AC:
        return "hardDiskAc";
    case STANDBY_AC:
        return "standbyAc";
    case HIBERNATE_AC:
        return "hibernateAc";
    case MONITOR_DC:
        return "monitorDc";
    case HARD_DISK_DC:
        return "hardDiskDc";
    case STANDBY_DC:
        return "standbyDc";
    case HIBERNATE_DC:
        return "hibernateDc";
    }

    return "";
}

}
