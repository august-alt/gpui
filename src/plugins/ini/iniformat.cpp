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

IniFormat::IniFormat()
    : PolicyFileFormat("ini")
{
}

bool IniFormat::read(std::istream &input, IniFile *file)
{
    try {
        std::string string_input(std::istreambuf_iterator<char>(input), {});
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff,
            std::codecvt_mode::little_endian>, char16_t> convertor;

        auto utf8 = convertor.to_bytes(reinterpret_cast<const char16_t*>(string_input.c_str()));

        // Strip BOM
        if (utf8[0] == '\xef'
         && utf8[1] == '\xbb'
         && utf8[2] == '\xbf')
        {
           utf8 = utf8.substr(3);
        }

        std::istringstream utf8stream(utf8);

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

bool IniFormat::write(std::ostream &output, IniFile *file)
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

        std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff,
                std::codecvt_mode::little_endian>, char16_t> convert;

        auto utf16le = convert.from_bytes(string_output.str());

        // Add BOM
        output.write("\xff\xfe", 2);
        output.write(reinterpret_cast<const char*>(utf16le.data()), utf16le.size() * 2);
    }
    catch (std::exception& e)
    {
        setErrorString(std::string("Error: ") + e.what());

        return false;
    }

    return true;
}

}
