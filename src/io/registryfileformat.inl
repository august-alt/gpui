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

class RegistryFileFormatPrivate
{
public:
    std::string name;
    std::string error;
};

template <typename TRegistryFile>
std::string RegistryFileFormat<TRegistryFile>::getName()
{
    return d->name;
}

template <typename TRegistryFile>
std::string RegistryFileFormat<TRegistryFile>::getErrorString()
{
    return d->error;
}

template <typename TRegistryFile>
RegistryFileFormat<TRegistryFile>* RegistryFileFormat<TRegistryFile>::create(const std::string& name)
{
    Q_UNUSED(name);
    return nullptr;
}

template <typename TRegistryFile>
std::vector<std::string> RegistryFileFormat<TRegistryFile>::getFormats()
{
    return std::vector<std::string>();
}

template <typename TRegistryFile>
void RegistryFileFormat<TRegistryFile>::setErrorString(const std::string& error)
{
    d->error = error;
}

template <typename TRegistryFile>
RegistryFileFormat<TRegistryFile>::RegistryFileFormat(const std::string &name)
    : d(new RegistryFileFormatPrivate())
{
    d->name = name;
}

template <typename TRegistryFile>
RegistryFileFormat<TRegistryFile>::~RegistryFileFormat()
{
    delete d;
}

}
