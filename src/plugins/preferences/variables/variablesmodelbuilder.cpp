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

#include "variablesmodelbuilder.h"

#include "variablescontaineritem.h"
#include "variablesitem.h"

#include "common/commonitem.h"

namespace preferences
{
VariablesModelBuilder::VariablesModelBuilder()
    : BaseModelBuilder()
{}

std::unique_ptr<PreferencesModel> VariablesModelBuilder::schemaToModel(
    std::unique_ptr<EnvironmentVariables> &variablesSource)
{
    auto model = std::make_unique<PreferencesModel>();

    for (const auto &variablesSchema : variablesSource->EnvironmentVariable())
    {
        auto properties = variablesSchema.Properties();
        for (const auto &currentProperties : properties)
        {
            int actionState = getActionCheckboxState(getOptionalPropertyData(currentProperties.action()).c_str());

            auto sessionItem = model->insertItem<VariablesContainerItem>(model->rootItem());
            sessionItem->setupListeners();

            auto variables = sessionItem->getVariables();
            variables->setProperty(VariablesItem::ACTION, actionState);
            variables->setProperty(VariablesItem::USER,
                                   static_cast<bool>(getOptionalPropertyData(currentProperties.user())));
            variables->setProperty(VariablesItem::NAME, currentProperties.name().c_str());
            variables->setProperty(VariablesItem::PARTIAL,
                                   static_cast<bool>(getOptionalPropertyData(currentProperties.partial())));
            variables->setProperty(VariablesItem::VALUE, currentProperties.value().c_str());
            variables->setProperty(VariablesItem::SYSTEM,
                                   static_cast<bool>(!getOptionalPropertyData(currentProperties.user())));

            auto common = sessionItem->getCommon();
            setCommonItemData(common, variablesSchema);
        }
    }

    return model;
}

std::unique_ptr<EnvironmentVariables> VariablesModelBuilder::modelToSchema(std::unique_ptr<PreferencesModel> &model)
{
    auto variables = std::make_unique<EnvironmentVariables>("{BF141A63-327B-438a-B9BF-2C188F13B7AD}");

    for (const auto &containerItem : model->topItems())
    {
        if (auto variablesContainer = dynamic_cast<VariablesContainerItem *>(containerItem); variablesContainer)
        {
            auto varModel    = variablesContainer->getVariables();
            auto commonModel = variablesContainer->getCommon();

            auto var = EnvironmentVariable_t("", "", "");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CLSID),
                                     "{78570023-8373-4a19-BA80-2F150738EA19}");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CHANGED), createDateOfChange());
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::NAME),
                                     varModel->property<std::string>(VariablesItem::NAME));

            auto properties = EnvironmentVariableProperties_t(varModel->property<std::string>(VariablesItem::NAME),
                                                              varModel->property<std::string>(VariablesItem::VALUE));
            properties.action(getActionCheckboxModel(varModel->property<int>(VariablesItem::ACTION)));
            properties.user(varModel->property<bool>(VariablesItem::USER));
            properties.partial(varModel->property<bool>(VariablesItem::PARTIAL));

            setCommonModelData(var, commonModel);
            var.Properties().push_back(properties);

            variables->EnvironmentVariable().push_back(var);
        }
    }

    return variables;
}

} // namespace preferences
