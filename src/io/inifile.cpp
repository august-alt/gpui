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

#include "inifile.h"

namespace io {

class IniFilePrivate
{
public:
    std::shared_ptr<IniFile::sections> sections;

    IniFilePrivate()
        : sections(std::make_shared<IniFile::sections>())
    {
    }
};


IniFile::IniFile()
    : d(new IniFilePrivate())
{
}

IniFile::~IniFile()
{
    delete d;
}

void IniFile::addValue(const std::string &fileSection, const std::string &key, const std::string &value)
{
    if (d->sections->find(fileSection) == d->sections->end())
    {
        (*d->sections)[fileSection] = IniFile::section();
    }

    (*d->sections)[fileSection].insert(key, value);
}

void IniFile::removeKey(const std::string &fileSection, const std::string &key)
{
    if (d->sections->find(fileSection) == d->sections->end())
    {
        return;
    }

    auto& currentSection = (*d->sections)[fileSection];

    auto targetKey = currentSection.find(key);

    if (targetKey != currentSection.end())
    {
        currentSection.erase(targetKey);
    }
}

std::shared_ptr<IniFile::sections> IniFile::getAllSections()
{
    return d->sections;
}

}
