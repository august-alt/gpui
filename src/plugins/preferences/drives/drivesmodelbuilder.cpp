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

#include "drivesmodelbuilder.h"

#include "drivescontaineritem.h"
#include "drivesitem.h"

#include "common/commonitem.h"

namespace preferences
{
DrivesModelBuilder::DrivesModelBuilder()
    : BaseModelBuilder()
{}

std::unique_ptr<PreferencesModel> DrivesModelBuilder::schemaToModel(std::unique_ptr<Drives> &drivesSource)
{
    auto model = std::make_unique<PreferencesModel>();

    for (const auto &drivesSchema : drivesSource->Drive())
    {
        auto properties = drivesSchema.Properties();
        for (const auto &currentProperties : properties)
        {
            int thisDriveIndex = getDrivesCheckboxIndex(getOptionalPropertyData(currentProperties.thisDrive()).c_str());
            int allDrivesIndex = getDrivesCheckboxIndex(getOptionalPropertyData(currentProperties.allDrives()).c_str());

            int actionState = getActionCheckboxState(getOptionalPropertyData(currentProperties.action()).c_str());

            auto sessionItem = model->insertItem<DrivesContainerItem>(model->rootItem());
            sessionItem->setupListeners();

            auto drives = sessionItem->getDrives();
            drives->setProperty(DrivesItem::ACTION, actionState);
            drives->setProperty(DrivesItem::PATH, currentProperties.path().c_str());
            drives->setProperty(DrivesItem::PERSISTENT, static_cast<bool>(currentProperties.persistent()));
            drives->setProperty(DrivesItem::LABEL, getOptionalPropertyData(currentProperties.label()).c_str());
            drives->setProperty(DrivesItem::LETTER, currentProperties.letter().c_str());
            drives->setProperty(DrivesItem::USER_NAME, getOptionalPropertyData(currentProperties.userName()).c_str());
            drives->setProperty(DrivesItem::CPASSWORD, getOptionalPropertyData(currentProperties.cpassword()).c_str());
            drives->setProperty(DrivesItem::USE_LETTER, static_cast<bool>(currentProperties.useLetter()));
            drives->setProperty(DrivesItem::THIS_DRIVE, thisDriveIndex);
            drives->setProperty(DrivesItem::ALL_DRIVES, allDrivesIndex);
            drives->setProperty(DrivesItem::USE_EXISTING, !currentProperties.useLetter());

            auto common = sessionItem->getCommon();
            setCommonItemData(common, drivesSchema);
        }
    }

    return model;
}

std::unique_ptr<Drives> DrivesModelBuilder::modelToSchema(std::unique_ptr<PreferencesModel> &model)
{
    auto drives = std::make_unique<Drives>("{8FDDCC1A-0C3C-43cd-A6B4-71A6DF20DA8C}");

    for (const auto &containerItem : model->topItems())
    {
        if (auto drivesContainer = dynamic_cast<DrivesContainerItem *>(containerItem); drivesContainer)
        {
            auto driveModel  = drivesContainer->getDrives();
            auto commonModel = drivesContainer->getCommon();

            auto drive = Drive_t("", "", "");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CLSID),
                                     "{935D1B74-9CB8-4e3c-9914-7DD559B7A417}");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CHANGED), createDateOfChange());
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::NAME),
                                     driveModel->property<std::string>(DrivesItem::PATH));

            auto properties = DriveProperties_t(driveModel->property<std::string>(DrivesItem::PATH),
                                                driveModel->property<bool>(DrivesItem::PERSISTENT),
                                                driveModel->property<bool>(DrivesItem::USE_LETTER),
                                                driveModel->property<std::string>(DrivesItem::LETTER));
            properties.action(getActionCheckboxModel(driveModel->property<int>(DrivesItem::ACTION)));
            properties.path(driveModel->property<std::string>(DrivesItem::PATH));
            properties.label(driveModel->property<std::string>(DrivesItem::LABEL));
            properties.userName(driveModel->property<std::string>(DrivesItem::USER_NAME));
            properties.cpassword(driveModel->property<std::string>(DrivesItem::CPASSWORD));
            properties.thisDrive(getDrivesCheckboxModel(driveModel->property<int>(DrivesItem::THIS_DRIVE)));
            properties.allDrives(getDrivesCheckboxModel(driveModel->property<int>(DrivesItem::ALL_DRIVES)));

            drive.Properties().push_back(properties);
            setCommonModelData(drive, commonModel);

            drives->Drive().push_back(drive);
        }
    }

    return drives;
}

int DrivesModelBuilder::getDrivesCheckboxIndex(const std::string &data)
{
    if (data.compare("NOCHANGE") == 0)
    {
        return 0;
    }

    if (data.compare("HIDE") == 0)
    {
        return 1;
    }

    if (data.compare("SHOW") == 0)
    {
        return 2;
    }

    return 0;
}

std::string DrivesModelBuilder::getDrivesCheckboxModel(const int index)
{
    switch (index)
    {
    case 1:
        return "HIDE";
    case 2:
        return "SHOW";
    default:
        break;
    }

    return "NOCHANGE";
}

} // namespace preferences
