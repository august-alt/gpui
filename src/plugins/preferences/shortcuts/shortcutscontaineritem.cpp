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

#include "shortcutscontaineritem.h"

#include "common/defaultactions.h"

#include "common/commonitem.h"
#include "shortcutsitem.h"

#include <mvvm/model/externalproperty.h>
#include <mvvm/signals/itemmapper.h>

#include "../../../core/logger/log.h"

namespace preferences
{
ShortcutsContainerItem::ShortcutsContainerItem()
    : ModelView::CompoundItem("ShortcutsContainerItem")
{
    addProperty(SHORTCUT_PATH, "")->setDisplayName(QObject::tr("Name").toStdString())->setEditable(false);
    addProperty(ORDER, 0)->setDisplayName(QObject::tr("Order").toStdString())->setEditable(false);
    addProperty(ACTION, defaultActionsToString(CREATE__MODE))
        ->setDisplayName(QObject::tr("Action").toStdString())
        ->setEditable(false);
    addProperty(TARGET_PATH, "")->setDisplayName(QObject::tr("Target").toStdString())->setEditable(false);

    addProperty<CommonItem>(COMMON)->setVisible(false);
    addProperty<ShortcutsItem>(SHORTCUTS)->setVisible(false);
}

CommonItem *ShortcutsContainerItem::getCommon() const
{
    return static_cast<CommonItem *>(children()[childrenCount() - 2]);
}

void ShortcutsContainerItem::setCommon(const CommonItem &item)
{
    setProperty(COMMON, item);
}

ShortcutsItem *ShortcutsContainerItem::getShortcuts() const
{
    return static_cast<ShortcutsItem *>(children().back());
}

void ShortcutsContainerItem::setShortcuts(const ShortcutsItem &item)
{
    setProperty(SHORTCUTS, item);
}

void ShortcutsContainerItem::setupListeners()
{
    auto onChildPropertyChange = [&](SessionItem *item, std::string property) {
        if (auto shortcutsItem = dynamic_cast<ShortcutsItem *>(item))
        {
            if (property == ACTION)
            {
                setProperty(ACTION, defaultActionsToString(shortcutsItem->property<int>(ACTION)));
            }

            if (property == SHORTCUT_PATH)
            {
                setProperty(SHORTCUT_PATH, shortcutsItem->property<std::string>(SHORTCUT_PATH));
            }

            if (property == TARGET_PATH)
            {
                setProperty(TARGET_PATH, shortcutsItem->property<std::string>(TARGET_PATH));
            }
        }
    };

    this->mapper()->setOnChildPropertyChange(onChildPropertyChange, nullptr);
}

void ShortcutsContainerItem::retranslateStrings()
{
    children()[0]->setDisplayName(QObject::tr("Name").toStdString());
    children()[1]->setDisplayName(QObject::tr("Order").toStdString());
    children()[2]->setDisplayName(QObject::tr("Action").toStdString());
    children()[3]->setDisplayName(QObject::tr("Target").toStdString());

    auto shortcutsItem = getShortcuts();

    setProperty(ACTION, defaultActionsToString(shortcutsItem->property<int>(ACTION)));
}

} // namespace preferences
