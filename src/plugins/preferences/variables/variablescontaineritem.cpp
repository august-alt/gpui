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

#include "variablescontaineritem.h"

#include "common/commonitem.h"
#include "variablesitem.h"

#include <mvvm/signals/itemmapper.h>

namespace preferences
{
VariablesContainerItem::VariablesContainerItem()
    : ModelView::CompoundItem("VariablesContainerItem")
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, "")->setDisplayName(QObject::tr("Action").toStdString())->setEditable(false);
    addProperty(VALUE, "")->setDisplayName(QObject::tr("Value").toStdString())->setEditable(false);
    addProperty(USER, QObject::tr("No").toStdString())
        ->setDisplayName(QObject::tr("User").toStdString())
        ->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<VariablesItem>(VARIABLES)->setVisible(false);
}

CommonItem *VariablesContainerItem::getCommon() const
{
    return static_cast<CommonItem *>(children()[childrenCount() - 2]);
}

void VariablesContainerItem::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

VariablesItem *VariablesContainerItem::getVariables() const
{
    return static_cast<VariablesItem *>(children().back());
}

void VariablesContainerItem::setVariables(const VariablesItem &item)
{
    setProperty(VARIABLES, item);
}

void VariablesContainerItem::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem *item, std::string property) {
        if (auto variablesItem = dynamic_cast<VariablesItem *>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, variablesItem->property<std::string>(ACTION));
            }

            if (property == VALUE)
            {
                setProperty(VALUE, variablesItem->property<std::string>(VALUE));
            }

            if (property == USER)
            {
                setProperty(USER,
                            variablesItem->property<bool>(USER) ? QObject::tr("Yes").toStdString()
                                                                : QObject::tr("No").toStdString());
            }

            if (property == NAME)
            {
                setProperty(NAME, variablesItem->property<std::string>(NAME));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

void VariablesContainerItem::retranslateStrings()
{
    children()[0]->setDisplayName(QObject::tr("Name").toStdString());
    children()[1]->setDisplayName(QObject::tr("Order").toStdString());
    children()[2]->setDisplayName(QObject::tr("Action").toStdString());
    children()[3]->setDisplayName(QObject::tr("Value").toStdString());
    children()[4]->setDisplayName(QObject::tr("User").toStdString());

    auto variablesItem = getVariables();

    setProperty(USER,
                variablesItem->property<bool>(USER) ? QObject::tr("Yes").toStdString()
                                                    : QObject::tr("No").toStdString());
}

} // namespace preferences
