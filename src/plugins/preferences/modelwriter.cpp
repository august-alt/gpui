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

#include "modelwriter.h"

#include "files/filespreferencewriter.h"
#include "folders/folderpreferencewriter.h"
#include "ini/inipreferencewriter.h"
#include "interfaces/preferencesreaderinterface.h"
#include "registry/registrypreferencewriter.h"
#include "shares/sharespreferencewriter.h"
#include "shortcuts/shortcutspreferencewriter.h"
#include "variables/variablespreferencewriter.h"

#include <QDebug>
#include <QDir>

#include "../../../src/plugins/storage/smb/smbdirectory.h"

namespace preferences
{
ModelWriter::ModelWriter() {}

void ModelWriter::saveModels(const std::string &policyPath,
                             const std::string &policyType,
                             std::map<std::string, std::unique_ptr<PreferencesModel>> *map)
{
    std::map<std::string, std::unique_ptr<PreferenceWriterInterface>> writers;
    writers["Preferences/Files/"]                = std::make_unique<FilesPreferenceWriter>();
    writers["Preferences/Folders/"]              = std::make_unique<FolderPreferenceWriter>();
    writers["Preferences/IniFiles/"]             = std::make_unique<IniPreferenceWriter>();
    writers["Preferences/Registry/"]             = std::make_unique<RegistryPreferenceWriter>();
    writers["Preferences/NetworkShares/"]        = std::make_unique<SharesPreferenceWriter>();
    writers["Preferences/Shortcuts/"]            = std::make_unique<ShortcutsPreferenceWriter>();
    writers["Preferences/EnvironmentVariables/"] = std::make_unique<VariablesPreferenceWriter>();

    std::vector<std::string> fileNames = {
        "EnvironmentVariables.xml",
        "Files.xml",
        "Folders.xml",
        "IniFiles.xml",
        "NetworkShares.xml",
        "Registry.xml",
        "Shortcuts.xml",
    };

    auto topDirectoryPath = policyPath + "/" + policyType;

    createDirectory(topDirectoryPath);

    auto preferencesDirectoryPath = policyPath + "/" + policyType + "/" + "Preferences";

    createDirectory(preferencesDirectoryPath);

    auto fileIt = fileNames.begin();

    for (const auto &writerPair : writers)
    {
        auto &writer = writerPair.second;
        try
        {
            auto directoryPath = policyPath + "/" + policyType + "/" + writerPair.first;

            createDirectory(directoryPath);

            auto fullPath = directoryPath + (*fileIt);

            auto modelPair = map->find(writerPair.second->getType());
            if (modelPair != map->end())
            {
                writer->write(fullPath, modelPair->second);

                qWarning() << "Saving file: " << fullPath.c_str();
            }
        }
        catch (std::exception &ex)
        {
            qWarning() << ex.what();
        }

        ++fileIt;
    }
}

void ModelWriter::createDirectory(const std::string &directoryName)
{
    const QString path = QString::fromStdString(directoryName);

    if (path.startsWith("smb://"))
    {
        gpui::smb::SmbDirectory dir(path);

        if (!dir.exists())
        {
            dir.mkdir(path);
        }
    }
    else
    {
        QDir dir(path);

        if (!dir.exists())
        {
            dir.mkdir(path);
        }
    }
}

} // namespace preferences
