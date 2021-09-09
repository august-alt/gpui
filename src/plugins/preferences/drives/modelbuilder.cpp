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

namespace gpui
{

std::unique_ptr<QStandardItemModel> ModelBuilder::schemaToModel(std::unique_ptr<Drives> &drives)
{
    std::unique_ptr<QStandardItemModel> model = std::make_unique<QStandardItemModel>(0, DriveRoles::ALL_DRIVE_ROLES);

    for (size_t index = 0; index < drives->Drive().size(); ++index)
    {
        auto drive = drives->Drive()[index];

        setStandardValues(drive, index, model);

        auto properties = drive.Properties().front();

        if (properties.action().present())
        {
            createComboBoxValue(properties.action().get(), index, DriveRoles::ACTION, model, { "R", "U", "D" });
        }

        createItem(properties.path(), index, DriveRoles::PATH, model);
        createItem(properties.persistent(), index, DriveRoles::PERSISTENT, model);
        createItemIfExist(properties.label(), index, DriveRoles::LABEL, model);
        createItem(properties.letter(), index, DriveRoles::LETTER, model);
        createItemIfExist(properties.userName(), index, DriveRoles::USERNAME, model);
        createItemIfExist(properties.cpassword(), index, DriveRoles::ENCRYPTED_PASSWORD, model);
        createItem(properties.useLetter(), index, DriveRoles::USE_LETTER, model);
        createItemIfExist(properties.thisDrive(), index, DriveRoles::THIS_DRIVE, model);
        createItemIfExist(properties.allDrives(), index, DriveRoles::ALL_DRIVES, model);
    }

    return model;
}

std::unique_ptr<Drives> ModelBuilder::modelToSchema(std::unique_ptr<QStandardItemModel> &model)
{
    Q_UNUSED(model);

    // TODO: Implement.

    return nullptr;
}

}
