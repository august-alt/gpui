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

#ifndef GPUI_POWER_MODEL_BUILDER_H
#define GPUI_POWER_MODEL_BUILDER_H

#include <memory>

#include "common/basemodelbuilder.h"
#include "common/preferencesmodel.h"
#include "schemas/poweroptionsschema.h"

namespace preferences
{

class PowerOptionsItem;
class PowerPlanItem;
class PowerSchemeItem;

class PowerModelBuilder : public BaseModelBuilder
{
public:
    PowerModelBuilder();

    std::unique_ptr<PreferencesModel> schemaToModel(std::unique_ptr<PowerOptions>& power);

    std::unique_ptr<PowerOptions> modelToSchema(std::unique_ptr<PreferencesModel>& model);

private:
    template <typename SourceType, typename ItemType, typename PropertiesType>
    void processItems(SourceType &printersSource,
                      std::unique_ptr<PreferencesModel>& model,
                      std::function<void(ItemType*, PropertiesType)>& callback);

    void processPowerOptions(PowerOptionsItem *powerOptions, GlobalPowerOptionsProperties_t properties);

    void processPowerPlan(PowerPlanItem *powerPlan, GlobalPowerOptionsV2Properties_t properties);

    void processPowerScheme(PowerSchemeItem *powerScheme, PowerSchemeProperties_t properties);
};

}

#endif // GPUI_POWER_MODEL_BUILDER_H

