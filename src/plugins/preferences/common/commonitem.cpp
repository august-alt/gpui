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

#include "commonitem.h"

#include <QUuid>

namespace preferences
{

CommonItem::CommonItem()
    : BasePreferenceItem<::preferences::CommonItem>("CommonItem")
{
    addProperty(propertyToString(CLSID), "");
    addProperty(propertyToString(DISABLED), false);
    addProperty(propertyToString(NAME), "");
    addProperty(propertyToString(STATUS), "");
    addProperty(propertyToString(IMAGE), static_cast<int>(0));
    addProperty(propertyToString(CHANGED), "");
    addProperty(propertyToString(UID), QUuid::createUuid().toString().toStdString());
    addProperty(propertyToString(DESC), "");
    addProperty(propertyToString(BYPASS_ERRORS), false);
    addProperty(propertyToString(USER_CONTEXT), false);
    addProperty(propertyToString(REMOVE_POLICY), false);
}

CommonItem::CommonItem(const CommonItem &other)
    : BasePreferenceItem<::preferences::CommonItem>("CommonItem")
{
    copyProperty<std::string>(CLSID, other);
    copyProperty<bool>(DISABLED, other);
    copyProperty<std::string>(NAME, other);
    copyProperty<std::string>(STATUS, other);
    copyProperty<int>(IMAGE, other);
    copyProperty<std::string>(CHANGED, other);
    copyProperty<std::string>(UID, other);
    copyProperty<std::string>(DESC, other);
    copyProperty<bool>(BYPASS_ERRORS, other);
    copyProperty<bool>(USER_CONTEXT, other);
    copyProperty<bool>(REMOVE_POLICY, other);
}

QString CommonItem::name() const
{
    return property<QString>(propertyToString(NAME));
}

void CommonItem::setName(const QString &newName)
{
    setProperty(propertyToString(NAME), newName);
}

std::string CommonItem::changed() const
{
    return property<std::string>(propertyToString(CHANGED));
}

void CommonItem::setChanged(bool state)
{
    setProperty(propertyToString(CHANGED), state);
}

QString CommonItem::desc() const
{
    return property<QString>(propertyToString(DESC));
}

void CommonItem::setDesc(const QString &newDesc)
{
    setProperty(propertyToString(DESC), newDesc);
}

bool CommonItem::bypassErrors() const
{
    return property<bool>(propertyToString(BYPASS_ERRORS));
}

void CommonItem::setBypassErrors(bool state)
{
    setProperty(propertyToString(BYPASS_ERRORS), state);
}

bool CommonItem::userContext() const
{
    return property<bool>(propertyToString(USER_CONTEXT));
}

void CommonItem::setUserContext(bool state)
{
    setProperty(propertyToString(USER_CONTEXT), state);
}

bool CommonItem::removePolicy() const
{
    return property<bool>(propertyToString(REMOVE_POLICY));
}

void CommonItem::setRemovePolicy(bool state)
{
    setProperty(propertyToString(REMOVE_POLICY), state);
}

}
