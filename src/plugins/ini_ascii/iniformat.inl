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

#include "iniformat.h"

#include <codecvt>

#include <QDebug>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace io;

namespace gpui
{

IniFormatAscii::IniFormatAscii()
    : PolicyFileFormat("ini_ascii")
{
}

bool IniFormatAscii::read(std::istream &input, IniFile *file)
{
    try {
        QByteArray array;
        char buffer[128];
        while (!input.eof())
        {
            int size = input.read(buffer, sizeof(buffer)).gcount();
            array.append(buffer, size);
        }

        QString string = QString::fromLocal8Bit(array);

        std::istringstream utf8stream(string.toStdString());

        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(utf8stream, pt);

        for (auto& section : pt)
        {
            qDebug() << "[" << section.first.c_str() << "]\n";

            for (auto& key : section.second)
            {
                qDebug() << key.first.c_str() << "=" << key.second.get_value("").c_str() << "\n";

                file->addValue(section.first, key.first, key.second.get_value(""));
            }
        }
    }
    catch (std::exception& e)
    {
        setErrorString(std::string("Error: ") + e.what());

        return false;
    }

    return true;
}

bool IniFormatAscii::write(std::ostream &output, io::IniFile *file)
{
    try {
        boost::property_tree::ptree pt;

        QMap<std::string, QMultiMap<std::string, std::string>>::const_iterator section_iterator =
                file->getAllSections()->constBegin();
        while (section_iterator != file->getAllSections()->constEnd()) {
            qDebug() << "[" << section_iterator.key().c_str() << "]\n";

            auto keyIterator = section_iterator->constBegin();

            while (keyIterator != section_iterator->constEnd())
            {
                qDebug() << section_iterator.key().c_str() << "." << keyIterator.key().c_str()
                         << keyIterator.value().c_str() << "\n";
                pt.add(section_iterator.key() + "." + keyIterator.key(), keyIterator.value());
                ++keyIterator;
            }
            ++section_iterator;
        }

        std::stringstream string_output;

        boost::property_tree::ini_parser::write_ini(string_output, pt);

        output << string_output.rdbuf();
    }
    catch (std::exception& e)
    {
        setErrorString(std::string("Error: ") + e.what());

        return false;
    }

    return true;
}

}
