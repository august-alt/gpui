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

#include "common/commonitem.h"

#include <mvvm/signals/itemmapper.h>

namespace preferences
{

template <typename PowerItemType>
PowerContainerItem<PowerItemType>::PowerContainerItem()
    : ModelView::CompoundItem(typeid(PowerContainerItem<PowerItemType>).name())
{
    addProperty(NAME, PowerItemType().template property<std::string>(NAME))
            ->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, "N/A")->setDisplayName(QObject::tr("Action").toStdString())->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<PowerItemType>(POWER)->setVisible(false);
}

template <typename PowerItemType>
CommonItem* PowerContainerItem<PowerItemType>::getCommon() const
{
    return static_cast<CommonItem*>(children()[childrenCount() - 2]);
}

template <typename PowerItemType>
void PowerContainerItem<PowerItemType>::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

template <typename PowerItemType>
PowerItemType* PowerContainerItem<PowerItemType>::getPower() const
{
    return static_cast<PowerItemType*>(children().back());
}

template <typename PowerItemType>
void PowerContainerItem<PowerItemType>::setPower(const PowerItemType& item)
{
    setProperty(POWER, item);
}

template <typename PowerItemType>
void PowerContainerItem<PowerItemType>::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem* item, std::string property)
    {
        if (auto powerItem = dynamic_cast<PowerItemType*>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, powerItem->template property<std::string>(ACTION));
            }

            if (property == NAME)
            {
                setProperty(NAME, powerItem->template property<std::string>(NAME));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

}
