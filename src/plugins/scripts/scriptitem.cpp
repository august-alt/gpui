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

#include "scriptitem.h"

namespace scripts_plugin
{
ScriptItem::ScriptItem()
    : ModelView::CompoundItem("ScriptItem")
{
    addProperty(propertyToString(PATH), "")->setDisplayName(QObject::tr("Path").toStdString());
    addProperty(propertyToString(ARGUMENTS), "")->setDisplayName(QObject::tr("Arguments").toStdString());
}

ScriptItem::ScriptItem(const ScriptItem &other)
    : ModelView::CompoundItem("ScriptItem")
{
    addProperty(propertyToString(PATH), other.property<std::string>(propertyToString(PATH)));
    addProperty(propertyToString(ARGUMENTS), other.property<std::string>(propertyToString(ARGUMENTS)));
}

constexpr int ScriptItem::propertyToInt(PropertyType &type)
{
    return static_cast<int>(type);
}

void ScriptItem::retranslateStrings()
{
    children()[0]->setDisplayName(QObject::tr("Path").toStdString());
    children()[1]->setDisplayName(QObject::tr("Arguments").toStdString());
}

} // namespace scripts_plugin
