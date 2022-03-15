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

#include <sstream>

namespace io {

/*!
 * \class GenericFile genericfile.h
 * \brief The GenericFile class represents a generic file.
 * \ingroup io
 *
 * The GenericFile class provides a common interface for interaction with policy files.
 */

template <class File, class Format>
inline GenericFile<File, Format>::GenericFile()
    : m_format(nullptr)
    , m_fileName()
    , m_errorString()
    , m_data{}
{

}

template <class File, class Format>
inline void GenericFile<File, Format>::setFileName(const std::string& fileName)
{
    m_fileName = fileName;
}

template <class File, class Format>
inline bool GenericFile<File, Format>::setFileFormat(const std::string& formatName)
{
    Format* format = Format::create(formatName);
    if (!format) {
        std::ostringstream error;
        error << "Format " << formatName << " is not supported." << std::endl;
        setErrorString(error.str());
        return false;
    }

    setFileFormat(format);
    return true;
}

template<class File, class Format>
inline void GenericFile<File, Format>::setFileFormat(const Format* format)
{
    m_format.swap(format);
}

template <class File, class Format>
inline std::string GenericFile<File, Format>::getFileFormat()
{
    return m_format ? m_format->getName() : "";
}

template <class File, class Format>
inline std::string GenericFile<File, Format>::getErrorString()
{
    return m_errorString;
}

template <class File, class Format>
inline bool GenericFile<File, Format>::read()
{
    return false;
}

template <class File, class Format>
inline bool GenericFile<File, Format>::write()
{
    return false;
}

template <class File, class Format>
inline void GenericFile<File, Format>::setData(const std::string& key, QVariant value)
{
    m_data.insert(QString::fromStdString(key), value);
}

template <class File, class Format>
inline QVariant GenericFile<File, Format>::getData(const std::string& key)
{
    return m_data.take(QString::fromStdString(key));
}

template <class File, class Format>
inline void GenericFile<File, Format>::setErrorString(const std::string& error)
{
    m_errorString = error;
}

}
