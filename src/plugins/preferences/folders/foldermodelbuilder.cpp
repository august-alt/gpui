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

#include "foldermodelbuilder.h"

#include "foldercontaineritem.h"
#include "folderitem.h"

#include "common/commonitem.h"

namespace preferences
{
FolderModelBuilder::FolderModelBuilder()
    : BaseModelBuilder()
{}

std::unique_ptr<PreferencesModel> FolderModelBuilder::schemaToModel(std::unique_ptr<Folders> &folderSource)
{
    auto model = std::make_unique<PreferencesModel>();

    for (const auto &foldersSchema : folderSource->Folder())
    {
        auto sessionItem = model->insertItem<FolderContainerItem>(model->rootItem());
        sessionItem->setupListeners();

        for (const auto &properties : foldersSchema.Properties())
        {
            auto folders = sessionItem->getFolder();
            folders->setProperty(FolderItem::ACTION,
                                 getActionCheckboxState(getOptionalPropertyData(properties.action()).c_str()));
            folders->setProperty(FolderItem::PATH, properties.path().c_str());
            folders->setProperty(FolderItem::READONLY, static_cast<bool>(properties.readOnly()));
            folders->setProperty(FolderItem::ARCHIVE, static_cast<bool>(properties.archive()));
            folders->setProperty(FolderItem::HIDDEN, static_cast<bool>(properties.hidden()));
            folders->setProperty(FolderItem::DELETE_IGNORE_ERRORS,
                                 static_cast<bool>(getOptionalPropertyData(properties.deleteIgnoreErrors())));
            folders->setProperty(FolderItem::DELETE_FILES,
                                 static_cast<bool>(getOptionalPropertyData(properties.deleteFiles())));
            folders->setProperty(FolderItem::DELETE_SUB_FOLDERS,
                                 static_cast<bool>(getOptionalPropertyData(properties.deleteSubFolders())));
            folders->setProperty(FolderItem::DELETE_FOLDER,
                                 static_cast<bool>(getOptionalPropertyData(properties.deleteFolder())));
            folders->setProperty(FolderItem::DELETE_READ_ONLY,
                                 static_cast<bool>(getOptionalPropertyData(properties.deleteReadOnly())));

            auto common = sessionItem->getCommon();
            setCommonItemData(common, foldersSchema);
        }
    }

    return model;
}

std::unique_ptr<Folders> FolderModelBuilder::modelToSchema(std::unique_ptr<PreferencesModel> &model)
{
    auto folders = std::make_unique<Folders>("{77CC39E7-3D16-4f8f-AF86-EC0BBEE2C861}");

    for (const auto &containerItem : model->topItems())
    {
        if (auto foldersContainer = dynamic_cast<FolderContainerItem *>(containerItem); foldersContainer)
        {
            auto folderModel = foldersContainer->getFolder();
            auto commonModel = foldersContainer->getCommon();

            auto folder = Folder_t("", "", "");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CLSID),
                                     "{07DA02F5-F9CD-4397-A550-4AE21B6B4BD3}");
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::CHANGED), createDateOfChange());
            commonModel->setProperty(CommonItem::propertyToString(CommonItem::NAME),
                                     foldersContainer->property<std::string>(FolderContainerItem::NAME));

            auto properties = FolderProperties_t(folderModel->property<std::string>(FolderItem::PATH),
                                                 folderModel->property<bool>(FolderItem::READONLY),
                                                 folderModel->property<bool>(FolderItem::ARCHIVE),
                                                 folderModel->property<bool>(FolderItem::HIDDEN));
            properties.action(getActionCheckboxModel(folderModel->property<int>(FolderItem::ACTION)));
            properties.deleteIgnoreErrors(folderModel->property<bool>(FolderItem::DELETE_IGNORE_ERRORS));
            properties.deleteFiles(folderModel->property<bool>(FolderItem::DELETE_FILES));
            properties.deleteSubFolders(folderModel->property<bool>(FolderItem::DELETE_SUB_FOLDERS));
            properties.deleteFolder(folderModel->property<bool>(FolderItem::DELETE_FOLDER));
            properties.deleteReadOnly(folderModel->property<bool>(FolderItem::DELETE_READ_ONLY));

            setCommonModelData(folder, commonModel);
            folder.Properties().push_back(properties);

            folders->Folder().push_back(folder);
        }
    }

    return folders;
}

} // namespace preferences
