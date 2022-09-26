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

#include "powermodelbuilder.h"

#include "powercontaineritem.h"
#include "poweroptionsitem.h"
#include "powerplanitem.h"
#include "powerschemeitem.h"

#include "common/commonitem.h"

namespace preferences
{

PowerModelBuilder::PowerModelBuilder()
    : BaseModelBuilder()
{
}

template <typename SourceType, typename ItemType, typename PropertiesType>
void PowerModelBuilder::processItems(SourceType &printersSource,
                  std::unique_ptr<PreferencesModel>& model,
                  std::function<void(ItemType*, PropertiesType)>& callback)
{
    for (const auto& currentSchema : printersSource)
    {
        auto sessionItem = model->insertItem<PowerContainerItem<ItemType>>(model->rootItem());

        for (const auto& properties : currentSchema.Properties())
        {
            auto* item = sessionItem->getPower();

            callback(item, properties);

            auto common = sessionItem->getCommon();
            setCommonItemData(common, currentSchema);
        }
    }
}

void PowerModelBuilder::processPowerOptions(PowerOptionsItem *powerOptions, GlobalPowerOptionsProperties_t properties)
{
    std::string closeLid = getOptionalPropertyData(properties.closeLid()).c_str();
    std::string pressPowerBtn = getOptionalPropertyData(properties.pressPowerBtn()).c_str();
    std::string pressSleepBtn = getOptionalPropertyData(properties.pressSleepBtn()).c_str();
    bool showIcon = getOptionalPropertyData(properties.showIcon());
    bool promptPassword = getOptionalPropertyData(properties.promptPassword());
    bool enableHibernation = getOptionalPropertyData(properties.enableHibernation());

    powerOptions->setProperty(PowerOptionsItem::propertyToString(PowerOptionsItem::CLOSE_LID), closeLid);
    powerOptions->setProperty(PowerOptionsItem::propertyToString(PowerOptionsItem::PRESS_POWER_BUTTON), pressPowerBtn);
    powerOptions->setProperty(PowerOptionsItem::propertyToString(PowerOptionsItem::PRESS_SLEEP_BUTTON), pressSleepBtn);
    powerOptions->setProperty(PowerOptionsItem::propertyToString(PowerOptionsItem::SHOW_ICON), showIcon);
    powerOptions->setProperty(PowerOptionsItem::propertyToString(PowerOptionsItem::PROMPT_PASSWORD), promptPassword);
    powerOptions->setProperty(PowerOptionsItem::propertyToString(PowerOptionsItem::ENABLE_HIBERNATION), enableHibernation);
}

void PowerModelBuilder::processPowerPlan(PowerPlanItem *powerPlan, GlobalPowerOptionsV2Properties_t properties)
{
    Q_UNUSED(powerPlan);
    Q_UNUSED(properties);
}

void PowerModelBuilder::processPowerScheme(PowerSchemeItem *powerScheme, PowerSchemeProperties_t properties)
{
    std::string action = getOptionalPropertyData(properties.action()).c_str();
    std::string name = properties.name().c_str();
    bool defaultScheme = getOptionalPropertyData(properties.default_());

    int monitorAc = getOptionalPropertyData(properties.monitorAc());
    int hardDiskAc = getOptionalPropertyData(properties.hardDiskAc());
    int standbyAc = getOptionalPropertyData(properties.standbyAc());
    int hibernateAc = getOptionalPropertyData(properties.hibernateAc());

    int monitorDc = getOptionalPropertyData(properties.monitorDc());
    int hardDiskDc = getOptionalPropertyData(properties.hardDiskDc());
    int standbyDc = getOptionalPropertyData(properties.standbyDc());
    int hibernateDc = getOptionalPropertyData(properties.hibernateDc());

    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::ACTION), action);
    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::NAME), name);
    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::DEFAULT), defaultScheme);
    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::MONITOR_AC), monitorAc);
    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::HARD_DISK_AC), hardDiskAc);
    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::STANDBY_AC), standbyAc);
    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::HIBERNATE_AC), hibernateAc);
    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::MONITOR_DC), monitorDc);
    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::HARD_DISK_DC), hardDiskDc);
    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::STANDBY_DC), standbyDc);
    powerScheme->setProperty(PowerSchemeItem::propertyToString(PowerSchemeItem::HIBERNATE_DC), hibernateDc);
}

std::unique_ptr<PreferencesModel> PowerModelBuilder::schemaToModel(std::unique_ptr<PowerOptions> &power)
{
    auto model = std::make_unique<PreferencesModel>();

    std::function<void(PowerOptionsItem*, GlobalPowerOptionsProperties_t)> powerOptionsHandler
            = [&](PowerOptionsItem *powerOptions, GlobalPowerOptionsProperties_t properties)
            {
                processPowerOptions(powerOptions, properties);
            };

    std::function<void(PowerPlanItem*, GlobalPowerOptionsV2Properties_t)> powerPlanHandler
            = [&](PowerPlanItem* powerPlan, GlobalPowerOptionsV2Properties_t properties)
            {
                processPowerPlan(powerPlan, properties);
            };

    std::function<void(PowerSchemeItem*, PowerSchemeProperties_t)> powerSchemeHandler
            = [&](PowerSchemeItem* powerScheme, PowerSchemeProperties_t properties)
            {
                processPowerScheme(powerScheme, properties);
            };

    processItems<PowerOptions::GlobalPowerOptionsSequence, PowerOptionsItem, GlobalPowerOptionsProperties_t>(
                power->GlobalPowerOptions(), model, powerOptionsHandler);

    processItems<PowerOptions::GlobalPowerOptionsV2Sequence, PowerPlanItem, GlobalPowerOptionsV2Properties_t>(
                power->GlobalPowerOptionsV2(), model, powerPlanHandler);

    processItems<PowerOptions::PowerSchemeSequence, PowerSchemeItem, PowerSchemeProperties_t>(
                power->PowerScheme(), model, powerSchemeHandler);

    return model;
}

std::unique_ptr<PowerOptions> PowerModelBuilder::modelToSchema(std::unique_ptr<PreferencesModel> &model)
{
    Q_UNUSED(model);

    return nullptr;
}

}

