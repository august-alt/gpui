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

template <typename PrinterItemType>
PrinterContainerItem<PrinterItemType>::PrinterContainerItem()
    : ModelView::CompoundItem(typeid(PrinterContainerItem<PrinterItemType>).name())
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, "")->setDisplayName(QObject::tr("Action").toStdString())->setEditable(false);
    addProperty(PATH, QObject::tr("N/A").toStdString())->setDisplayName(QObject::tr("Path").toStdString())->setEditable(false);
    addProperty(DEFAULT, QObject::tr("N/A").toStdString())->setDisplayName(QObject::tr("Default").toStdString())->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<PrinterItemType>(PRINTER)->setVisible(false);
}

template <typename PrinterItemType>
CommonItem* PrinterContainerItem<PrinterItemType>::getCommon() const
{
    return static_cast<CommonItem*>(children()[childrenCount() - 2]);
}

template <typename PrinterItemType>
void PrinterContainerItem<PrinterItemType>::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

template <typename PrinterItemType>
PrinterItemType* PrinterContainerItem<PrinterItemType>::getPrinter() const
{
    return static_cast<PrinterItemType*>(children().back());
}

template <typename PrinterItemType>
void PrinterContainerItem<PrinterItemType>::setPrinter(const PrinterItemType &item)
{
    setProperty(PRINTER, item);
}

template <typename PrinterItemType>
void PrinterContainerItem<PrinterItemType>::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem* item, std::string property)
    {
        if (auto printerItem = dynamic_cast<PrinterItemType*>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, printerItem->template property<std::string>(ACTION));
            }

            if (property == NAME)
            {
                setProperty(NAME, printerItem->template property<std::string>(NAME));
            }

            if (property == PATH)
            {
                setProperty(PATH, printerItem->template property<std::string>(PATH));
            }

            if (property == DEFAULT)
            {
                setProperty(DEFAULT, printerItem->template property<bool>(DEFAULT)
                            ? "Yes"
                            : "No");
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

}

