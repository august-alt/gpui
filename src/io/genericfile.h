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

#ifndef GPUI_GENERICFILE_H
#define GPUI_GENERICFILE_H

#include "io.h"

#include <iostream>
#include <memory>
#include <string>

#include <QVariant>

namespace io {
    template <class File, class Format>
    class GPUI_IO_EXPORT GenericFile
    {
    public:
        GenericFile();

        virtual ~GenericFile() = default;

        void setFileName(const std::string& fileName);

        bool setFileFormat(const std::string& formatName);

        void setFileFormat(const Format* format);

        std::string getFileFormat();

        std::string getErrorString();

        bool read();

        bool write();

        void setData(const std::string& key, QVariant value);

        QVariant getData(const std::string& key);

    protected:
        void setErrorString(const std::string& error);

    private:
        std::unique_ptr<Format> m_format;

        std::string m_fileName;

        std::string m_errorString;

        QVariantMap m_data;
    };
}

#include "genericfile.inl"

#endif // GPUI_GENERICFILE_H
