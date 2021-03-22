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

#include "genericfile.h"

namespace io {

template <class File, class Format>
void GenericFile<File, Format>::setFileName(const std::string& fileName)
{
    Q_UNUSED(fileName);
}

template <class File, class Format>
void GenericFile<File, Format>::setFileFormat(const std::string& format)
{
    Q_UNUSED(format);
}

template <class File, class Format>
std::string GenericFile<File, Format>::getFileFormat()
{
    return "";
}

template <class File, class Format>
std::string GenericFile<File, Format>::getErrorString()
{
    return "";
}

template <class File, class Format>
bool GenericFile<File, Format>::read()
{
    return false;
}

template <class File, class Format>
bool GenericFile<File, Format>::write()
{
    return false;
}

template <class File, class Format>
void GenericFile<File, Format>::setData(const std::string& key, QVariant value)
{
    Q_UNUSED(key);
    Q_UNUSED(value);
}

template <class File, class Format>
QVariant GenericFile<File, Format>::getData(const std::string& key)
{
    Q_UNUSED(key);
    return QVariant();
}

template <class File, class Format>
void GenericFile<File, Format>::setErrorString(const std::string& error)
{
    Q_UNUSED(error);
}

}
