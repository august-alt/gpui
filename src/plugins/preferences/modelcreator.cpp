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

#include "modelcreator.h"

#include "../../core/logger/log.h"

#include <fstream>

#include "drives/drivespreferencereader.h"
#include "files/filespreferencereader.h"
#include "folders/folderpreferencereader.h"
#include "ini/inipreferencereader.h"
#include "interfaces/preferencesreaderinterface.h"
#include "registry/registrypreferencereader.h"
#include "shares/sharespreferencereader.h"
#include "shortcuts/shortcutspreferencereader.h"
#include "variables/variablespreferencereader.h"

namespace preferences
{
ModelCreator::ModelCreator() {}

void ModelCreator::populateModels(const std::string &policyPath,
                                  const std::string &policyType,
                                  std::map<std::string, std::unique_ptr<PreferencesModel>> *map)
{
    std::map<std::string, std::unique_ptr<PreferenceReaderInterface>> readers;
    readers["Preferences/Drives/Drives.xml"]               = std::make_unique<DrivesPreferenceReader>();
    readers["Preferences/Files/Files.xml"]                 = std::make_unique<FilesPreferenceReader>();
    readers["Preferences/Folders/Folders.xml"]             = std::make_unique<FolderPreferenceReader>();
    readers["Preferences/IniFiles/IniFiles.xml"]           = std::make_unique<IniPreferenceReader>();
    readers["Preferences/Registry/Registry.xml"]           = std::make_unique<RegistryPreferenceReader>();
    readers["Preferences/NetworkShares/NetworkShares.xml"] = std::make_unique<SharesPreferenceReader>();
    readers["Preferences/Shortcuts/Shortcuts.xml"]         = std::make_unique<ShortcutsPreferenceReader>();
    readers["Preferences/EnvironmentVariables/EnvironmentVariables.xml"] = std::make_unique<VariablesPreferenceReader>();

    for (const auto &readerPair : readers)
    {
        auto &reader = readerPair.second;
        try
        {
            auto model = reader->read(policyPath + "/" + policyType + "/" + readerPair.first);
            if (model.get())
            {
                map->insert(
                    std::pair<std::string, std::unique_ptr<PreferencesModel>>(reader->getType(), std::move(model)));
            }
            else
            {
                model = std::make_unique<PreferencesModel>();

                map->insert(
                    std::pair<std::string, std::unique_ptr<PreferencesModel>>(reader->getType(), std::move(model)));
            }
        }
        catch (std::exception &ex)
        {
            GPUI_WARNING_STREAM << ex.what();
        }
    }
}

} // namespace preferences
