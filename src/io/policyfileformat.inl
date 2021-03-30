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
};

template <typename TPolicyFile>
std::string PolicyFileFormat<TPolicyFile>::getName()
{
    return std::string();
}

template <typename TPolicyFile>
void PolicyFileFormat<TPolicyFile>::read(std::istream& input, TPolicyFile* file)
{
    Q_UNUSED(input);
    Q_UNUSED(file);
}

template <typename TPolicyFile>
void PolicyFileFormat<TPolicyFile>::write(std::istream& output, TPolicyFile *file)
{
    Q_UNUSED(output);
    Q_UNUSED(file);
}

template <typename TPolicyFile>
std::string PolicyFileFormat<TPolicyFile>::getErrorString()
{
    return std::string();
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
    Q_UNUSED(error);
}

template <typename TPolicyFile>
PolicyFileFormat<TPolicyFile>::PolicyFileFormat(const std::string &name)
    : d(new PolicyFileFormatPrivate())
{
    Q_UNUSED(name);
}

template <typename TPolicyFile>
PolicyFileFormat<TPolicyFile>::~PolicyFileFormat()
{
    delete d;
}


}
