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

#include "poweroptionsitem.h"

namespace preferences
{

PowerOptionsItem::PowerOptionsItem()
    : BasePreferenceItem<::preferences::PowerOptionsItem>("PowerOptionsItem")
{
    addProperty(propertyToString(CLOSE_LID), "");
    addProperty(propertyToString(PRESS_POWER_BUTTON), "");
    addProperty(propertyToString(PRESS_SLEEP_BUTTON), "");
    addProperty(propertyToString(SHOW_ICON), false);
    addProperty(propertyToString(PROMPT_PASSWORD), false);
    addProperty(propertyToString(ENABLE_HIBERNATION), false);
    addProperty(propertyToString(NAME), QObject::tr("Power Options (Windows XP)").toStdString());
}

PowerOptionsItem::PowerOptionsItem(const PowerOptionsItem &other)
    : BasePreferenceItem<::preferences::PowerOptionsItem>("PowerOptionsItem")
{
    copyProperty<std::string>(CLOSE_LID, other);
    copyProperty<std::string>(PRESS_POWER_BUTTON, other);
    copyProperty<std::string>(PRESS_SLEEP_BUTTON, other);
    copyProperty<bool>(SHOW_ICON, other);
    copyProperty<bool>(PROMPT_PASSWORD, other);
    copyProperty<bool>(ENABLE_HIBERNATION, other);
    copyProperty<std::string>(NAME, other);
}

}

