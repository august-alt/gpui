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

#include "inicontaineritem.h"

#include "common/commonitem.h"
#include "iniitem.h"

#include <mvvm/signals/itemmapper.h>

namespace preferences
{

IniContainerItem::IniContainerItem()
    : ModelView::CompoundItem("IniContainerItem")
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, "")->setDisplayName(QObject::tr("Action").toStdString())->setEditable(false);
    addProperty(PATH, "")->setDisplayName(QObject::tr("Path").toStdString())->setEditable(false);
    addProperty(SECTION, "")->setDisplayName(QObject::tr("Section").toStdString())->setEditable(false);
    addProperty(PROPERTY, "")->setDisplayName(QObject::tr("Property").toStdString())->setEditable(false);
    addProperty(VALUE, "")->setDisplayName(QObject::tr("Value").toStdString())->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<IniItem>(INI)->setVisible(false);
}

CommonItem* IniContainerItem::getCommon() const
{
    return static_cast<CommonItem*>(children()[childrenCount() - 2]);
}

void IniContainerItem::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

IniItem *IniContainerItem::getIni() const
{
    return static_cast<IniItem*>(children().back());
}

void IniContainerItem::setIni(const IniItem &item)
{
    setProperty(INI, item);
}

void IniContainerItem::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem* item, std::string property)
    {
        if (auto iniItem = dynamic_cast<IniItem*>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, iniItem->property<std::string>(ACTION));
            }

            if (property == PATH)
            {
                auto name = QUrl(QString::fromStdString(iniItem->property<std::string>(PATH))).fileName().toStdString();
                setProperty(NAME, name);
                setProperty(PATH, iniItem->property<std::string>(PATH));
            }

            if (property == SECTION)
            {
                setProperty(SECTION, iniItem->property<std::string>(SECTION));
            }

            if (property == PROPERTY)
            {
                setProperty(PROPERTY, iniItem->property<std::string>(PROPERTY));
            }

            if (property == VALUE)
            {
                setProperty(VALUE, iniItem->property<std::string>(VALUE));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

}
