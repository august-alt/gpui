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

#include "modelbuilder.h"
#include "shortcutroles.h"

#include <QStandardItem>
#include <QStandardItemModel>

#include <QDebug>

namespace gpui {

std::unique_ptr<QStandardItemModel> ModelBuilder::schemaToModel(std::unique_ptr<Shortcuts> &shortcuts)
{
    std::unique_ptr<QStandardItemModel> model = std::make_unique<QStandardItemModel>(0, ShortcutRoles::ALL_SHORTCUT_ROLES);

    for (size_t index = 0; index < shortcuts->Shortcut().size(); ++index)
    {
        auto shortcut = shortcuts->Shortcut()[index];

        setStandardValues(shortcut, index, model);

        auto properties = shortcut.Properties().front();

        if (properties.action().present())
        {
            createComboBoxValue(properties.action().get(), index, ShortcutRoles::ACTION, model, { "R", "U", "D" });
        }

        createItemIfExist(properties.arguments(), index, ShortcutRoles::ARGUMENTS, model);

        createItemIfExist(properties.comment(), index, ShortcutRoles::COMMENT, model);

        createItemIfExist(properties.iconIndex(), index, ShortcutRoles::ICON_INDEX, model);

        createItemIfExist(properties.iconPath(), index, ShortcutRoles::ICON_PATH, model);

        createItemIfExist(properties.pidl(), index, ShortcutRoles::PIDL, model);

        createItemIfExist(properties.startIn(), index, ShortcutRoles::START_IN, model);

        createItemIfExist(properties.shortcutKey(), index, ShortcutRoles::SHORTCUT_KEY, model);

        createItem(properties.shortcutPath(), index, ShortcutRoles::SHORTCUT_PATH, model);

        createItem(properties.targetPath(), index, ShortcutRoles::TARGET_PATH, model);

        createComboBoxValue(properties.targetType(), index, ShortcutRoles::TARGET_TYPE, model, { "URL", "SHELL" });

        createItemIfExist(properties.window(), index, ShortcutRoles::WINDOW, model);
    }

    return model;
}

std::unique_ptr<Shortcuts> ModelBuilder::modelToSchema(std::unique_ptr<QStandardItemModel> &model)
{
    Q_UNUSED(model);

    // TODO: Implement.

    return nullptr;
}

}
