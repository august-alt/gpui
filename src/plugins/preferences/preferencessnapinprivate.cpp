/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
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

#include "preferencessnapinprivate.h"

#include "interfaces/containeriteminterface.h"

namespace gpui
{
using namespace preferences;

PreferencesSnapInPrivate::PreferencesSnapInPrivate()
    : machinePreferencesModels(new PreferencesModelMap())
    , userPreferencesModels(new PreferencesModelMap())
{}

void PreferencesSnapInPrivate::onDataSave()
{
    auto modelWriter = std::make_unique<ModelWriter>();

    modelWriter->saveModels(this->policyPath, "Machine", this->machinePreferencesModels.get());
    modelWriter->saveModels(this->policyPath, "User", this->userPreferencesModels.get());
}

void PreferencesSnapInPrivate::retranslateModels(std::unique_ptr<PreferencesSnapInPrivate::PreferencesModelMap> &models)
{
    if (models)
    {
        for (auto &dataModel : (*models))
        {
            for (auto &item : dataModel.second->topItems())
            {
                auto containerItem = dynamic_cast<ContainerItemInterface *>(item);
                {
                    containerItem->retranslateStrings();
                }
            }
        }
    }
}

} // namespace gpui
