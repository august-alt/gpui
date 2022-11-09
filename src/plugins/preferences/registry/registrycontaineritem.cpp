/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
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

#include "registrycontaineritem.h"

#include "common/commonitem.h"
#include "registryitem.h"

#include <mvvm/signals/itemmapper.h>

namespace preferences
{
RegistryContainerItem::RegistryContainerItem()
    : ModelView::CompoundItem("RegistryContainerItem")
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, "")->setDisplayName(QObject::tr("Action").toStdString())->setEditable(false);
    addProperty(HIVE, "")->setDisplayName(QObject::tr("Hive").toStdString())->setEditable(false);
    addProperty(KEY, "")->setDisplayName(QObject::tr("Key").toStdString())->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<RegistryItem>(REGISTRY)->setVisible(false);
}

CommonItem *RegistryContainerItem::getCommon() const
{
    return static_cast<CommonItem *>(children()[childrenCount() - 2]);
}

void RegistryContainerItem::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

RegistryItem *RegistryContainerItem::getRegistry() const
{
    return static_cast<RegistryItem *>(children().back());
}

void RegistryContainerItem::setRegistry(const RegistryItem &item)
{
    setProperty(REGISTRY, item);
}

void RegistryContainerItem::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem *item, std::string property) {
        if (auto registryItem = dynamic_cast<RegistryItem *>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, registryItem->property<std::string>(ACTION));
            }

            if (property == NAME)
            {
                setProperty(NAME, registryItem->property<std::string>(NAME));
            }

            if (property == HIVE)
            {
                setProperty(HIVE, registryItem->property<std::string>(HIVE));
            }

            if (property == KEY)
            {
                setProperty(KEY, registryItem->property<std::string>(KEY));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

void RegistryContainerItem::retranslateStrings()
{
    children()[0]->setDisplayName(QObject::tr("Name").toStdString());
    children()[1]->setDisplayName(QObject::tr("Order").toStdString());
    children()[2]->setDisplayName(QObject::tr("Action").toStdString());
    children()[3]->setDisplayName(QObject::tr("Hive").toStdString());
    children()[4]->setDisplayName(QObject::tr("Key").toStdString());
}

} // namespace preferences
