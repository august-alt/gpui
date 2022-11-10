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

#include "genericreader.h"

#include "../core/pluginstorage.h"

#include <fstream>

#include <QDebug>
#include <QString>

namespace io
{
template<typename TData, typename TFormat>
std::unique_ptr<TData> GenericReader::load(const std::string &fileName)
{
    std::unique_ptr<TData> fileData;

    QString pluginName = QString::fromStdString(fileName);
    pluginName         = pluginName.mid(pluginName.lastIndexOf('.') + 1);

    TFormat *format = gpui::PluginStorage::instance()->createPluginClass<TFormat>(pluginName);

    if (!format)
    {
        qWarning() << "Format supporting: " << pluginName << " not found.";

        return fileData;
    }

    std::ifstream file;

    file.open(fileName, std::ifstream::in);

    if (file.good())
    {
        fileData = std::make_unique<TData>();

        if (!format->read(file, fileData.get()))
        {
            qWarning() << fileName.c_str() << " " << format->getErrorString().c_str();
        }
    }

    file.close();

    delete format;

    return fileData;
}

template<typename TData, typename TFormat>
std::unique_ptr<TData> GenericReader::load(std::istream &fileContent, const std::string &pluginName)
{
    std::unique_ptr<TData> fileData;

    TFormat *format = gpui::PluginStorage::instance()->createPluginClass<TFormat>(QString::fromStdString(pluginName));

    if (!format)
    {
        qWarning() << "Format supporting: " << pluginName.c_str() << " not found.";

        return fileData;
    }

    if (fileContent.good())
    {
        fileData = std::make_unique<TData>();

        if (!format->read(fileContent, fileData.get()))
        {
            qWarning() << "Error while reading file contents: " << format->getErrorString().c_str();
        }
    }

    delete format;

    return fileData;
}

} // namespace io
