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

#include "foldercontaineritem.h"

#include "common/commonitem.h"
#include "folderitem.h"

#include <mvvm/signals/itemmapper.h>

namespace preferences
{
FolderContainerItem::FolderContainerItem()
    : ModelView::CompoundItem("FolderContainerItem")
{
    addProperty(NAME, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, "")->setDisplayName(QObject::tr("Action").toStdString())->setEditable(false);
    addProperty(PATH, "")->setDisplayName(QObject::tr("Path").toStdString())->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<FolderItem>(FOLDER)->setVisible(false);
}

CommonItem *FolderContainerItem::getCommon() const
{
    return static_cast<CommonItem *>(children()[childrenCount() - 2]);
}

void FolderContainerItem::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

FolderItem *FolderContainerItem::getFolder() const
{
    return static_cast<FolderItem *>(children().back());
}

void FolderContainerItem::setFolder(const FolderItem &item)
{
    setProperty(FOLDER, item);
}

void FolderContainerItem::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem *item, std::string property) {
        if (auto filesItem = dynamic_cast<FolderItem *>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, filesItem->property<std::string>(ACTION));
            }

            if (property == PATH)
            {
                auto name = QUrl(QString::fromStdString(filesItem->property<std::string>(PATH))).fileName().toStdString();
                setProperty(NAME, name);
                setProperty(PATH, filesItem->property<std::string>(PATH));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

void FolderContainerItem::retranslateStrings()
{
    children()[0]->setDisplayName(QObject::tr("Name").toStdString());
    children()[1]->setDisplayName(QObject::tr("Order").toStdString());
    children()[2]->setDisplayName(QObject::tr("Action").toStdString());
    children()[3]->setDisplayName(QObject::tr("Path").toStdString());
}

} // namespace preferences
