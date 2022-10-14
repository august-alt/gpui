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

#include <QDebug>

namespace preferences
{
ModelWriter::ModelWriter() {}

void ModelWriter::saveModels(const std::string &policyPath,
                             const std::string &policyType,
                             std::map<std::string, std::unique_ptr<PreferencesModel>> *map)
{
    Q_UNUSED(map);

    std::map<std::string, std::unique_ptr<int>> writers;
    writers["Files/Files.xml"]                               = std::make_unique<int>();
    writers["Folders/Folders.xml"]                           = std::make_unique<int>();
    writers["IniFiles/IniFiles.xml"]                         = std::make_unique<int>();
    writers["Registry/Registry.xml"]                         = std::make_unique<int>();
    writers["NetworkShares/NetworkShares.xml"]               = std::make_unique<int>();
    writers["Shortcuts/Shortcuts.xml"]                       = std::make_unique<int>();
    writers["EnvironmentVariables/EnvironmentVariables.xml"] = std::make_unique<int>();

    for (const auto &writerPair : writers)
    {
        auto &writer = writerPair.second;
        try
        {
            auto fullPath = policyPath + "/" + policyType + "/" + writerPair.first;
            Q_UNUSED(fullPath);
            Q_UNUSED(writer);

            // TODO: Implement writing.
        }
        catch (std::exception &ex)
        {
            qWarning() << ex.what();
        }
    }
}

} // namespace preferences
