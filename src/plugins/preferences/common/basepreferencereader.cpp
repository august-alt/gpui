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
#include <QDebug>

std::string preferences::BasePreferenceReader::getType() const
{
    return readerType;
}

std::unique_ptr<preferences::PreferencesModel> preferences::BasePreferenceReader::read(const std::string path)
{
    std::unique_ptr<PreferencesModel> result;

    std::ifstream file;

    file.open(path, std::ifstream::in);

    bool ok = file.good();
    if (!ok)
    {
        qWarning() << "Failed to read file: " << path.c_str();
        return nullptr;
    }

    try
    {
        result = createModel(file);
    }
    catch (const std::exception &e)
    {
        qWarning() << e.what();
    }

    file.close();

    return result;
}

preferences::BasePreferenceReader::BasePreferenceReader(const std::string &type)
{
    readerType = type;
}
