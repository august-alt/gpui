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

#include "basepreferencereader.h"

#include <fstream>
#include <sstream>
#include "../../../core/logger/log.h"

#include "../../../src/plugins/storage/smb/smbfile.h"

std::string preferences::BasePreferenceReader::getType() const
{
    return readerType;
}

std::unique_ptr<preferences::PreferencesModel> preferences::BasePreferenceReader::read(const std::string path)
{
    std::unique_ptr<PreferencesModel> result;

    auto stringvalues = std::make_unique<std::string>();

    try
    {
        QString qtPath = QString::fromStdString(path);

        if (qtPath.startsWith("smb://"))
        {
            gpui::smb::SmbFile smbFile(qtPath);
            smbFile.open(QFile::ReadOnly);
            stringvalues->resize(smbFile.size(), 0);
            smbFile.read(&stringvalues->at(0), smbFile.size());
            smbFile.close();
        }
        else
        {
            QFile registryFile(qtPath);
            registryFile.open(QFile::ReadOnly);
            stringvalues->resize(registryFile.size(), 0);
            registryFile.read(&stringvalues->at(0), registryFile.size());
            registryFile.close();
        }

        auto iss = std::make_unique<std::istringstream>(*stringvalues);

        result = createModel(*iss.get());
    }
    catch (const std::exception &e)
    {
        GPUI_WARNING_STREAM << e.what();
    }

    return result;
}

preferences::BasePreferenceReader::BasePreferenceReader(const std::string &type)
{
    readerType = type;
}
