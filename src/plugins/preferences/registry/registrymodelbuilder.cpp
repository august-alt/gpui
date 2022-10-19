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

#include "registrymodelbuilder.h"

#include "registrycontaineritem.h"
#include "registryitem.h"

#include "common/commonitem.h"

namespace preferences
{
RegistryModelBuilder::RegistryModelBuilder()
    : BaseModelBuilder()
{}

std::unique_ptr<PreferencesModel> RegistryModelBuilder::schemaToModel(std::unique_ptr<RegistrySettings> &registry)
{
    auto model = std::make_unique<PreferencesModel>();

    for (const auto &registrySchema : registry->Registry())
    {
        auto sessionItem = model->insertItem<RegistryContainerItem>(model->rootItem());
        sessionItem->setupListeners();

        for (const auto &properties : registrySchema.Properties())
        {
            auto registryKey = sessionItem->getRegistry();
            registryKey->setProperty(RegistryItem::ACTION,
                                     getActionCheckboxState(getOptionalPropertyData(properties.action()).c_str()));
            registryKey->setProperty(RegistryItem::HIVE, properties.hive().c_str());
            registryKey->setProperty(RegistryItem::KEY, properties.key().c_str());
            registryKey->setProperty(RegistryItem::NAME, getOptionalPropertyData(properties.name()).c_str());
            registryKey->setProperty(RegistryItem::TYPE, getOptionalPropertyData(properties.type()).c_str());
            registryKey->setProperty(RegistryItem::VALUE, getOptionalPropertyData(properties.value()).c_str());
            registryKey->setProperty(RegistryItem::DEFAULT, getOptionalPropertyData(properties.default_()));

            auto common = sessionItem->getCommon();
            setCommonItemData(common, registrySchema);
        }
    }

    return model;
}

std::unique_ptr<RegistrySettings> RegistryModelBuilder::modelToSchema(std::unique_ptr<PreferencesModel> &model)
{
    auto registryKeys = std::make_unique<RegistrySettings>("{A3CCFC41-DFDB-43a5-8D26-0FE8B954DA51}");

    for (const auto &containerItem : model->topItems())
    {
        if (auto registryContainer = dynamic_cast<RegistryContainerItem *>(containerItem))
        {
            auto registryModel = registryContainer->getRegistry();
            auto commonModel   = registryContainer->getCommon();

            auto registryKey = Registry_t("", "", "");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CLSID),
                                     "{9CD4B2F4-923D-47f5-A062-E897DD1DAD50}");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CHANGED), createDateOfChange());

            auto properties = RegistryProperties_t(SubProp_t("", 0, 0),
                                                   registryModel->property<std::string>(RegistryItem::HIVE),
                                                   registryModel->property<std::string>(RegistryItem::KEY));
            properties.action(registryModel->property<std::string>(RegistryItem::ACTION));
            properties.name(registryModel->property<std::string>(RegistryItem::NAME));
            properties.type(registryModel->property<std::string>(RegistryItem::TYPE));
            properties.value(registryModel->property<std::string>(RegistryItem::VALUE));
            properties.default_(registryModel->property<bool>(RegistryItem::DEFAULT));

            setCommonModelData(registryKey, commonModel);
            registryKey.Properties().push_back(properties);

            registryKeys->Registry().push_back(registryKey);
        }
    }

    return registryKeys;
}

} // namespace preferences
