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

#include "shortcutsmodelbuilder.h"

#include "shortcutscontaineritem.h"
#include "shortcutsitem.h"

#include "common/commonitem.h"

namespace preferences
{
ShortcutsModelBuilder::ShortcutsModelBuilder()
    : BaseModelBuilder()
{}

std::unique_ptr<PreferencesModel> ShortcutsModelBuilder::schemaToModel(
    std::unique_ptr<Shortcuts> &shortcutsSource)
{
    auto model = std::make_unique<PreferencesModel>();

    for (const auto &shortcutsSchema : shortcutsSource->Shortcut())
    {
        auto properties = shortcutsSchema.Properties();
        for (const auto &currentProperties : properties)
        {
            std::string actionState = getActionCheckboxState(
                getOptionalPropertyData(currentProperties.action()).c_str());
            int iconIndex = static_cast<int>(getOptionalPropertyData(currentProperties.iconIndex()));

            auto sessionItem = model->insertItem<ShortcutsContainerItem>(model->rootItem());

            auto shortcuts = sessionItem->getShortcuts();
            shortcuts->setProperty(ShortcutsItem::ACTION, actionState);
            shortcuts->setProperty(ShortcutsItem::PIDL,
                                   getOptionalPropertyData(currentProperties.pidl()).c_str());
            shortcuts->setProperty(ShortcutsItem::SHORTCUT_PATH,
                                   currentProperties.shortcutPath().c_str());
            shortcuts->setProperty(ShortcutsItem::TARGET_TYPE,
                                   currentProperties.targetType().c_str());
            shortcuts->setProperty(ShortcutsItem::TARGET_PATH,
                                   currentProperties.targetPath().c_str());
            shortcuts->setProperty(ShortcutsItem::ARGUMENTS,
                                   getOptionalPropertyData(currentProperties.arguments()).c_str());
            shortcuts->setProperty(ShortcutsItem::START_IN,
                                   getOptionalPropertyData(currentProperties.startIn()).c_str());
            shortcuts->setProperty(ShortcutsItem::WINDOW,
                                   getOptionalPropertyData(currentProperties.window()).c_str());
            shortcuts->setProperty(ShortcutsItem::COMMENT,
                                   getOptionalPropertyData(currentProperties.comment()).c_str());
            shortcuts->setProperty(ShortcutsItem::ICON_PATH,
                                   getOptionalPropertyData(currentProperties.iconPath()).c_str());
            shortcuts->setProperty(ShortcutsItem::ICON_INDEX, std::to_string(iconIndex));

            auto shortcutKey = getOptionalPropertyData(currentProperties.shortcutKey());

            shortcuts->setProperty(ShortcutsItem::SHORTCUT_KEY, decodeShortcutKey(shortcutKey));

            auto common = sessionItem->getCommon();
            setCommonItemData(common, shortcutsSchema);
        }
    }

    return model;
}

std::unique_ptr<Shortcuts> ShortcutsModelBuilder::modelToSchema(
    std::unique_ptr<PreferencesModel> &model)
{
    auto shortcuts = std::make_unique<Shortcuts>("{872ECB34-B2EC-401b-A585-D32574AA90EE}");

    for (const auto &containerItem : model->topItems())
    {
        if (auto shortcutsContainer = dynamic_cast<ShortcutsContainerItem *>(containerItem);
            shortcutsContainer)
        {
            auto shortcutModel = shortcutsContainer->getShortcuts();
            auto commonModel   = shortcutsContainer->getCommon();

            std::string key = shortcutModel->property<std::string>(ShortcutsItem::SHORTCUT_KEY);

            auto shortcut = createRootElement<Shortcut_t>("{4F2F7C55-2790-433e-8127-0739D1CFA327}");

            auto properties = ShortcutsProperties_t(shortcutModel->property<std::string>(
                                                        ShortcutsItem::TARGET_TYPE),
                                                    shortcutModel->property<std::string>(
                                                        ShortcutsItem::TARGET_PATH),
                                                    shortcutModel->property<std::string>(
                                                        ShortcutsItem::SHORTCUT_PATH));
            properties.action(shortcutModel->property<std::string>(ShortcutsItem::ACTION));
            properties.pidl(shortcutModel->property<std::string>(ShortcutsItem::PIDL));
            properties.arguments(shortcutModel->property<std::string>(ShortcutsItem::ARGUMENTS));
            properties.startIn(shortcutModel->property<std::string>(ShortcutsItem::START_IN));
            properties.window(shortcutModel->property<std::string>(ShortcutsItem::WINDOW));
            properties.comment(shortcutModel->property<std::string>(ShortcutsItem::COMMENT));
            properties.iconPath(shortcutModel->property<std::string>(ShortcutsItem::ICON_PATH));
            properties.iconIndex(
                std::stoi(shortcutModel->property<std::string>(ShortcutsItem::ICON_INDEX)));
            properties.shortcutKey(encodeShortcutKey(key));

            setCommonModelData(shortcut, commonModel);
            shortcut.Properties().push_back(properties);

            shortcuts->Shortcut().push_back(shortcut);
        }
    }

    return shortcuts;
}

std::string ShortcutsModelBuilder::decodeShortcutKey(unsigned char shortcutKey)
{
    Q_UNUSED(shortcutKey);

    return "";
}

unsigned char ShortcutsModelBuilder::encodeShortcutKey(std::string shortcutKey)
{
    Q_UNUSED(shortcutKey);

    return 0;
}

} // namespace preferences
