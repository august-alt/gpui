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
template<typename LocalItem>
LocalGroupContainerItem<LocalItem>::LocalGroupContainerItem()
    : ModelView::CompoundItem(typeid(LocalGroupContainerItem<LocalItem>).name())
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, "")->setDisplayName(QObject::tr("Action").toStdString())->setEditable(false);
    addProperty(FULL_NAME, QObject::tr("N/A").toStdString())
        ->setDisplayName(QObject::tr("Full Name").toStdString())
        ->setEditable(false);
    addProperty(DESCRIPTION, "")->setDisplayName(QObject::tr("Description").toStdString())->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<LocalItem>(LOCAL_USER_OR_GROUP)->setVisible(false);
}

template<typename LocalItem>
CommonItem *LocalGroupContainerItem<LocalItem>::getCommon() const
{
    return static_cast<CommonItem *>(children()[childrenCount() - 2]);
}

template<typename LocalItem>
void LocalGroupContainerItem<LocalItem>::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

template<typename LocalItem>
LocalItem LocalGroupContainerItem<LocalItem>::getLocalGroup() const
{
    return property<LocalItem>(LOCAL_USER_OR_GROUP);
}

template<typename LocalItem>
void LocalGroupContainerItem<LocalItem>::setLocalGroup(const LocalItem &item)
{
    setProperty(LOCAL_USER_OR_GROUP, item);
}

template<typename LocalItem>
void LocalGroupContainerItem<LocalItem>::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem *item, std::string property) {
        if (auto baseItem = dynamic_cast<LocalItem *>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, baseItem->template property<std::string>(ACTION));
            }

            if (property == GROUP_NAME)
            {
                setProperty(NAME, baseItem->template property<std::string>(GROUP_NAME));
            }

            if (property == USER_NAME)
            {
                setProperty(NAME, baseItem->template property<std::string>(USER_NAME));
            }

            if (property == DESCRIPTION)
            {
                setProperty(DESCRIPTION, baseItem->template property<std::string>(DESCRIPTION));
            }

            if (property == FULL_NAME)
            {
                setProperty(FULL_NAME, baseItem->template property<std::string>(FULL_NAME));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

template<typename LocalGroupItem>
void LocalGroupContainerItem<LocalGroupItem>::retranslateStrings()
{
    children()[0]->setDisplayName(QObject::tr("Name").toStdString());
    children()[1]->setDisplayName(QObject::tr("Order").toStdString());
    children()[2]->setDisplayName(QObject::tr("Action").toStdString());
    children()[3]->setDisplayName(QObject::tr("Full Name").toStdString());
    children()[4]->setDisplayName(QObject::tr("Description").toStdString());
}

} // namespace preferences
