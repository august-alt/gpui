/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
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

#include "policyfileformat.h"
#include <QtCore>

namespace io {

class PolicyFileFormatPrivate
{
public:
};

std::string PolicyFileFormat::getName()
{
    return std::string();
}

void PolicyFileFormat::read(std::istream& input, PolicyFile* file)
{
    Q_UNUSED(input);
    Q_UNUSED(file);
}

void PolicyFileFormat::write(std::istream& output, PolicyFile* file)
{
    Q_UNUSED(output);
    Q_UNUSED(file);
}

std::string PolicyFileFormat::getErrorString()
{
    return std::string();
}

PolicyFileFormat* PolicyFileFormat::create(const std::string& name)
{
    Q_UNUSED(name);
    return nullptr;
}

std::vector<std::string> PolicyFileFormat::getFormats()
{
    return std::vector<std::string>();
}

void PolicyFileFormat::setErrorString(const std::string& error)
{
    Q_UNUSED(error);
}

PolicyFileFormat::PolicyFileFormat(const std::string &name)
    : d(new PolicyFileFormatPrivate())
{
    Q_UNUSED(name);
}

}
