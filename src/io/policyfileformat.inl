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

#include <QtCore>

namespace io {

class PolicyFileFormatPrivate
{
public:
    std::string name;
    std::string error;
};

template <typename TPolicyFile>
std::string PolicyFileFormat<TPolicyFile>::getName()
{
    return d->name;
}

template <typename TPolicyFile>
std::string PolicyFileFormat<TPolicyFile>::getErrorString()
{
    return d->error;
}

template <typename TPolicyFile>
PolicyFileFormat<TPolicyFile>* PolicyFileFormat<TPolicyFile>::create(const std::string& name)
{
    Q_UNUSED(name);
    return nullptr;
}

template <typename TPolicyFile>
std::vector<std::string> PolicyFileFormat<TPolicyFile>::getFormats()
{
    return std::vector<std::string>();
}

template <typename TPolicyFile>
void PolicyFileFormat<TPolicyFile>::setErrorString(const std::string& error)
{
    d->error = error;
}

template <typename TPolicyFile>
PolicyFileFormat<TPolicyFile>::PolicyFileFormat(const std::string &name)
    : d(new PolicyFileFormatPrivate())
{
    d->name = name;
}

template <typename TPolicyFile>
PolicyFileFormat<TPolicyFile>::~PolicyFileFormat()
{
    delete d;
}


}
