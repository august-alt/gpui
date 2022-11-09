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

#include "sharescontaineritem.h"

#include "common/defaultactions.h"

#include "common/commonitem.h"
#include "sharesitem.h"

#include <mvvm/signals/itemmapper.h>

static inline const std::string LIMIT_USERS = "limitUsers";

namespace preferences
{
SharesContainerItem::SharesContainerItem()
    : ModelView::CompoundItem("SharesContainerItem")
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, defaultActionsToString(CREATE__MODE))
        ->setDisplayName(QObject::tr("Action").toStdString())
        ->setEditable(false);
    addProperty(PATH, "")->setDisplayName(QObject::tr("Path").toStdString())->setEditable(false);
    addProperty(USER_LIMIT, QObject::tr("Unchanged").toStdString())
        ->setDisplayName(QObject::tr("User Limit").toStdString())
        ->setEditable(false);
    addProperty(ACCESS_BASED_ENUMERATION, QObject::tr("Unchanged").toStdString())
        ->setDisplayName(QObject::tr("ABE").toStdString())
        ->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<SharesItem>(SHARES)->setVisible(false);
}

CommonItem *SharesContainerItem::getCommon() const
{
    return static_cast<CommonItem *>(children()[childrenCount() - 2]);
}

void SharesContainerItem::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

SharesItem *SharesContainerItem::getShares() const
{
    return static_cast<SharesItem *>(children().back());
}

void SharesContainerItem::setShares(const SharesItem &item)
{
    setProperty(SHARES, item);
}

void SharesContainerItem::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem *item, std::string property) {
        if (auto sharesItem = dynamic_cast<SharesItem *>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, defaultActionsToString(sharesItem->property<int>(ACTION)));
            }

            if (property == NAME)
            {
                setProperty(NAME, sharesItem->property<std::string>(NAME));
            }

            if (property == PATH)
            {
                setProperty(PATH, sharesItem->property<std::string>(PATH));
            }

            if (property == LIMIT_USERS)
            {
                auto limitUsers = sharesItem->property<std::string>(LIMIT_USERS);

                if (limitUsers.compare("NO_CHANGE") == 0)
                {
                    setProperty(USER_LIMIT, QObject::tr("Unchanged").toStdString());
                }
                else if (limitUsers.compare("MAX_ALLOWED") == 0)
                {
                    setProperty(USER_LIMIT, QObject::tr("Maximum").toStdString());
                }
                else
                {
                    setProperty(USER_LIMIT, QString::number(sharesItem->property<int>(USER_LIMIT)).toStdString());
                }
            }

            if (property == ACCESS_BASED_ENUMERATION)
            {
                auto abe = sharesItem->property<std::string>(ACCESS_BASED_ENUMERATION);

                if (abe.compare("NO_CHANGE") == 0)
                {
                    setProperty(ACCESS_BASED_ENUMERATION, QObject::tr("Unchanged").toStdString());
                }
                else if (abe.compare("ENABLE") == 0)
                {
                    setProperty(ACCESS_BASED_ENUMERATION, QObject::tr("Enabled").toStdString());
                }
                else
                {
                    setProperty(ACCESS_BASED_ENUMERATION, QObject::tr("Disabled").toStdString());
                }
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

void SharesContainerItem::retranslateStrings()
{
    children()[0]->setDisplayName(QObject::tr("Name").toStdString());
    children()[1]->setDisplayName(QObject::tr("Order").toStdString());
    children()[2]->setDisplayName(QObject::tr("Action").toStdString());
    children()[3]->setDisplayName(QObject::tr("Path").toStdString());
    children()[4]->setDisplayName(QObject::tr("User Limit").toStdString());
    children()[5]->setDisplayName(QObject::tr("ABE").toStdString());

    auto sharesItem = getShares();

    setProperty(ACTION, defaultActionsToString(sharesItem->property<int>(ACTION)));

    auto limitUsers = sharesItem->property<std::string>(LIMIT_USERS);

    if (limitUsers.compare("NO_CHANGE") == 0)
    {
        setProperty(USER_LIMIT, QObject::tr("Unchanged").toStdString());
    }
    else if (limitUsers.compare("MAX_ALLOWED") == 0)
    {
        setProperty(USER_LIMIT, QObject::tr("Maximum").toStdString());
    }
    else
    {
        setProperty(USER_LIMIT, QString::number(sharesItem->property<int>(USER_LIMIT)).toStdString());
    }

    auto abe = sharesItem->property<std::string>(ACCESS_BASED_ENUMERATION);

    if (abe.compare("NO_CHANGE") == 0)
    {
        setProperty(ACCESS_BASED_ENUMERATION, QObject::tr("Unchanged").toStdString());
    }
    else if (abe.compare("ENABLE") == 0)
    {
        setProperty(ACCESS_BASED_ENUMERATION, QObject::tr("Enabled").toStdString());
    }
    else
    {
        setProperty(ACCESS_BASED_ENUMERATION, QObject::tr("Disabled").toStdString());
    }
}

} // namespace preferences
