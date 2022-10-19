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

#include "inimodelbuilder.h"

#include "inicontaineritem.h"
#include "iniitem.h"

#include "common/commonitem.h"

namespace preferences
{
IniModelBuilder::IniModelBuilder()
    : BaseModelBuilder()
{}

std::unique_ptr<PreferencesModel> IniModelBuilder::schemaToModel(std::unique_ptr<IniFiles> &ini)
{
    auto model = std::make_unique<PreferencesModel>();

    for (const auto &iniSchema : ini->Ini())
    {
        for (const auto &properties : iniSchema.Properties())
        {
            auto sessionItem = model->insertItem<IniContainerItem>(model->rootItem());
            auto iniItem     = sessionItem->getIni();
            auto action      = getActionCheckboxState(getOptionalPropertyData(properties.action()).c_str());
            iniItem->setProperty(IniItem::ACTION, action);
            iniItem->setProperty(IniItem::PATH, properties.path().c_str());
            iniItem->setProperty(IniItem::SECTION, getOptionalPropertyData(properties.section()).c_str());
            iniItem->setProperty(IniItem::VALUE, getOptionalPropertyData(properties.value()).c_str());
            iniItem->setProperty(IniItem::PROPERTY, getOptionalPropertyData(properties.property()).c_str());

            auto common = sessionItem->getCommon();
            setCommonItemData(common, iniSchema);
        }
    }

    return model;
}

std::unique_ptr<IniFiles> IniModelBuilder::modelToSchema(std::unique_ptr<PreferencesModel> &model)
{
    auto drives = std::make_unique<IniFiles>("{694C651A-08F2-47fa-A427-34C4F62BA207}");

    for (const auto &containerItem : model->topItems())
    {
        if (auto iniContainer = dynamic_cast<IniContainerItem *>(containerItem); iniContainer)
        {
            auto iniModel    = iniContainer->getIni();
            auto commonModel = iniContainer->getCommon();

            auto ini = Ini_t("", "", "");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CLSID),
                                     "{EEFACE84-D3D8-4680-8D4B-BF103E759448}");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CHANGED), createDateOfChange());

            auto properties = IniProperties_t(iniModel->property<std::string>(IniItem::PATH));
            properties.action(iniModel->property<std::string>(IniItem::ACTION));
            properties.path(iniModel->property<std::string>(IniItem::PATH));
            properties.section(iniModel->property<std::string>(IniItem::SECTION));
            properties.value(iniModel->property<std::string>(IniItem::VALUE));
            properties.property(iniModel->property<std::string>(IniItem::PROPERTY));

            ini.Properties().push_back(properties);
            setCommonModelData(ini, commonModel);

            drives->Ini().push_back(ini);
        }
    }

    return drives;
}

} // namespace preferences
