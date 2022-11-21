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

#include <QDebug>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace io;

namespace gpui
{
IniFormat::IniFormat()
    : PolicyFileFormat("ini")
{}

bool IniFormat::read(std::istream &input, IniFile *file)
{
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(input, pt);

        for (auto &section : pt)
        {
            qDebug() << "[" << section.first.c_str() << "]\n";

            for (auto &key : section.second)
            {
                qDebug() << key.first.c_str() << "=" << key.second.get_value("").c_str() << "\n";

                file->addValue(section.first, key.first, key.second.get_value(""));
            }
        }
    }
    catch (std::exception &e)
    {
        setErrorString(std::string("Error: ") + e.what());

        return false;
    }

    return true;
}

bool IniFormat::write(std::ostream &output, IniFile *file)
{
    try
    {
        boost::property_tree::ptree pt;

        QMap<std::string, QMultiMap<std::string, std::string>>::const_iterator section_iterator
            = file->getAllSections()->constBegin();
        while (section_iterator != file->getAllSections()->constEnd())
        {
            qDebug() << "[" << section_iterator.key().c_str() << "]\n";

            auto keyIterator = section_iterator->constBegin();

            while (keyIterator != section_iterator->constEnd())
            {
                qDebug() << keyIterator->c_str() << "="
                         << section_iterator->value(*keyIterator).c_str() << "\n";
                pt.add(section_iterator.key() + "." + (*keyIterator),
                       section_iterator->value(*keyIterator));
                ++keyIterator;
            }
            ++section_iterator;
        }

        boost::property_tree::ini_parser::write_ini(output, pt);
    }
    catch (std::exception &e)
    {
        setErrorString(std::string("Error: ") + e.what());

        return false;
    }

    return true;
}

} // namespace gpui
