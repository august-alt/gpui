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

#include "devicecontaineritem.h"

#include "common/commonitem.h"
#include "deviceitem.h"

#include <mvvm/signals/itemmapper.h>

namespace preferences
{
DeviceContainerItem::DeviceContainerItem()
    : ModelView::CompoundItem("DeviceContainerItem")
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, "")->setDisplayName(QObject::tr("Action").toStdString())->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<DeviceItem>(DEVICE)->setVisible(false);
}

CommonItem DeviceContainerItem::getCommon() const
{
    return property<CommonItem>(COMMON);
}

void DeviceContainerItem::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

DeviceItem DeviceContainerItem::getDevice() const
{
    return property<DeviceItem>(DEVICE);
}

void DeviceContainerItem::setDevice(const DeviceItem &item)
{
    setProperty(DEVICE, item);
}

void DeviceContainerItem::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem *item, std::string property) {
        if (auto deviceItem = dynamic_cast<DeviceItem *>(item))
        {
            if (property == DeviceItem::DEVICE_ACTION)
            {
                setProperty(ACTION, deviceItem->property<std::string>(DeviceItem::DEVICE_ACTION));
            }

            if (property == DeviceItem::DEVICE_CLASS)
            {
                setProperty(NAME, deviceItem->property<std::string>(DeviceItem::DEVICE_CLASS));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

void DeviceContainerItem::retranslateStrings()
{
    children()[0]->setDisplayName(QObject::tr("Name").toStdString());
    children()[1]->setDisplayName(QObject::tr("Order").toStdString());
    children()[2]->setDisplayName(QObject::tr("Action").toStdString());
}

} // namespace preferences
