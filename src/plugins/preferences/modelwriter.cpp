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

namespace preferences
{
ModelWriter::ModelWriter() {}

void ModelWriter::saveModels(const std::string &policyPath,
                             const std::string &policyType,
                             std::map<std::string, std::unique_ptr<PreferencesModel>> *map)
{
    std::map<std::string, std::unique_ptr<PreferenceWriterInterface>> writers;
    writers["Files/Files.xml"]                               = std::make_unique<FilesPreferenceWriter>();
    writers["Folders/Folders.xml"]                           = std::make_unique<FolderPreferenceWriter>();
    writers["IniFiles/IniFiles.xml"]                         = std::make_unique<IniPreferenceWriter>();
    writers["Registry/Registry.xml"]                         = std::make_unique<RegistryPreferenceWriter>();
    writers["NetworkShares/NetworkShares.xml"]               = std::make_unique<SharesPreferenceWriter>();
    writers["Shortcuts/Shortcuts.xml"]                       = std::make_unique<ShortcutsPreferenceWriter>();
    writers["EnvironmentVariables/EnvironmentVariables.xml"] = std::make_unique<VariablesPreferenceWriter>();

    for (const auto &writerPair : writers)
    {
        auto &writer = writerPair.second;
        try
        {
            auto fullPath = policyPath + "/" + policyType + "/" + writerPair.first;

            auto modelPair = map->find(writerPair.first);
            if (modelPair != map->end())
            {
                writer->write(fullPath, modelPair->second);
            }
        }
        catch (std::exception &ex)
        {
            qWarning() << ex.what();
        }
    }
}

} // namespace preferences
