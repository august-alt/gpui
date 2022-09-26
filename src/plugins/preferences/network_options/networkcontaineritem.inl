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
#include "basenetworkitem.h"

#include <mvvm/signals/itemmapper.h>

namespace preferences
{

template <typename NetworkItem>
NetworkContainerItem<NetworkItem>::NetworkContainerItem()
    : ModelView::CompoundItem(typeid(NetworkContainerItem<NetworkItem>).name())
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, "")->setDisplayName(QObject::tr("Action").toStdString())->setEditable(false);
    addProperty(ADDRESS, "")->setDisplayName(QObject::tr("Address").toStdString())->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<NetworkItem>(NETWORK)->setVisible(false);
}

template <typename NetworkItem>
CommonItem* NetworkContainerItem<NetworkItem>::getCommon() const
{
    return static_cast<CommonItem*>(children()[childrenCount() - 2]);
}

template <typename NetworkItem>
void NetworkContainerItem<NetworkItem>::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

template <typename NetworkItem>
NetworkItem* NetworkContainerItem<NetworkItem>::getNetwork() const
{
    return static_cast<NetworkItem*>(children()[childrenCount() - 1]);
}

template <typename NetworkItem>
void NetworkContainerItem<NetworkItem>::setNetwork(const NetworkItem &item)
{
    setProperty(NETWORK, item);
}

template <typename NetworkItem>
void NetworkContainerItem<NetworkItem>::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem* item, std::string property)
    {
        if (auto networkItem = dynamic_cast<NetworkItem*>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, networkItem->template property<std::string>(ACTION));
            }

            if (property == NAME)
            {
                setProperty(NAME, networkItem->template property<std::string>(NAME));
            }

            if (property == IP_ADDRESS)
            {
                setProperty(ADDRESS, networkItem->template property<std::string>(IP_ADDRESS));
            }

            if (property == PHONE_NUMBER)
            {
                setProperty(ADDRESS, networkItem->template property<std::string>(PHONE_NUMBER));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

}
