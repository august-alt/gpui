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

#include "datasourcecontaineritem.h"

#include "common/defaultactions.h"

#include "common/commonitem.h"
#include "datasourceitem.h"

#include <mvvm/signals/itemmapper.h>

namespace preferences
{
DataSourceContainerItem::DataSourceContainerItem()
    : ModelView::CompoundItem("DataSourceContainerItem")
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, defaultActionsToString(CREATE__MODE))
        ->setDisplayName(QObject::tr("Action").toStdString())
        ->setEditable(false);
    addProperty(DRIVER, "")->setDisplayName(QObject::tr("Driver").toStdString())->setEditable(false);
    addProperty(USERDSN, "")->setDisplayName(QObject::tr("User DSN").toStdString())->setEditable(false);
    addProperty(USERNAME, "")->setDisplayName(QObject::tr("Username").toStdString())->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<DataSourceItem>(DATASOURCE)->setVisible(false);
}

CommonItem DataSourceContainerItem::getCommon() const
{
    return property<CommonItem>(COMMON);
}

void DataSourceContainerItem::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

DataSourceItem *DataSourceContainerItem::getDataSource() const
{
    return static_cast<DataSourceItem *>(children().back());
}

void DataSourceContainerItem::setDataSource(const DataSourceItem &item)
{
    setProperty(DATASOURCE, item);
}

void DataSourceContainerItem::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem *item, std::string property) {
        if (auto dataSourceItem = dynamic_cast<DataSourceItem *>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, defaultActionsToString(dataSourceItem->property<int>(ACTION)));
            }

            if (property == DSN)
            {
                setProperty(NAME, dataSourceItem->property<std::string>(DSN));
            }

            if (property == DRIVER)
            {
                setProperty(DRIVER, dataSourceItem->property<std::string>(DRIVER));
            }

            if (property == USERDSN)
            {
                setProperty(USERDSN, dataSourceItem->property<std::string>(USERDSN));
            }

            if (property == USERNAME)
            {
                setProperty(USERNAME, dataSourceItem->property<std::string>(USERNAME));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

void DataSourceContainerItem::retranslateStrings()
{
    children()[0]->setDisplayName(QObject::tr("Name").toStdString());
    children()[1]->setDisplayName(QObject::tr("Order").toStdString());
    children()[2]->setDisplayName(QObject::tr("Action").toStdString());
    children()[3]->setDisplayName(QObject::tr("Driver").toStdString());
    children()[4]->setDisplayName(QObject::tr("User DSN").toStdString());
    children()[5]->setDisplayName(QObject::tr("Username").toStdString());

    auto dataSourceItem = getDataSource();

    setProperty(ACTION, defaultActionsToString(dataSourceItem->property<int>(ACTION)));
}

} // namespace preferences
