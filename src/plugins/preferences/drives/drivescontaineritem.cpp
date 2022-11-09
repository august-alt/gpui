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

#include "drivescontaineritem.h"

#include "common/commonitem.h"
#include "drivesitem.h"

#include <mvvm/signals/itemmapper.h>

namespace preferences
{
DrivesContainerItem::DrivesContainerItem()
    : ModelView::CompoundItem("DrivesContainerItem")
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, "")->setDisplayName(QObject::tr("Action").toStdString())->setEditable(false);
    addProperty(PATH, "")->setDisplayName(QObject::tr("Path").toStdString())->setEditable(false);
    addProperty(PERSISTENT, QObject::tr("No").toStdString())
        ->setDisplayName(QObject::tr("Reconnect").toStdString())
        ->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<DrivesItem>(DRIVES)->setVisible(false);
}

CommonItem *DrivesContainerItem::getCommon() const
{
    return static_cast<CommonItem *>(children()[childrenCount() - 2]);
}

void DrivesContainerItem::setCommon(const CommonItem &item)
{
    setProperty<CommonItem>(COMMON, item);
}

DrivesItem *DrivesContainerItem::getDrives() const
{
    return static_cast<DrivesItem *>(children().back());
}

void DrivesContainerItem::setDrives(const DrivesItem &item)
{
    setProperty<DrivesItem>(DRIVES, item);
}

void DrivesContainerItem::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem *item, std::string property) {
        if (auto drivesItem = dynamic_cast<DrivesItem *>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, drivesItem->property<std::string>(ACTION));
            }

            if (property == PATH)
            {
                setProperty(NAME, drivesItem->property<std::string>(PATH));
                setProperty(PATH, drivesItem->property<std::string>(PATH));
            }

            if (property == PERSISTENT)
            {
                setProperty(PERSISTENT,
                            drivesItem->property<bool>(PERSISTENT) ? QObject::tr("Yes").toStdString()
                                                                   : QObject::tr("No").toStdString());
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

void DrivesContainerItem::retranslateStrings()
{
    children()[0]->setDisplayName(QObject::tr("Name").toStdString());
    children()[1]->setDisplayName(QObject::tr("Order").toStdString());
    children()[2]->setDisplayName(QObject::tr("Action").toStdString());
    children()[3]->setDisplayName(QObject::tr("Path").toStdString());
    children()[4]->setDisplayName(QObject::tr("Reconnect").toStdString());

    auto drivesItem = getDrives();

    setProperty(PERSISTENT,
                drivesItem->property<bool>(PERSISTENT) ? QObject::tr("Yes").toStdString()
                                                       : QObject::tr("No").toStdString());
}

} // namespace preferences
