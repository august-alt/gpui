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

#ifndef GPUI_REGISTRYFILEFORMAT_H
#define GPUI_REGISTRYFILEFORMAT_H

#include "io.h"

#include <memory>
#include <string>
#include <vector>

namespace io {
    class RegistryFileFormatPrivate;

    template <typename TRegistryFile>
    class GPUI_IO_EXPORT RegistryFileFormat
    {
    public:
        std::string getName();

        virtual bool read(std::istream& input, TRegistryFile* file) = 0;

        virtual bool write(std::ostream& output, TRegistryFile* file) = 0;

        std::string getErrorString();

        static RegistryFileFormat* create(const std::string& name);

        static std::vector<std::string> getFormats();

        virtual ~RegistryFileFormat();

    protected:
        void setErrorString(const std::string& error);

        RegistryFileFormat(const std::string& name);

    private:
        RegistryFileFormat(const RegistryFileFormat&)            = delete;   // copy ctor
        RegistryFileFormat(RegistryFileFormat&&)                 = delete;   // move ctor
        RegistryFileFormat& operator=(const RegistryFileFormat&) = delete;   // copy assignment
        RegistryFileFormat& operator=(RegistryFileFormat&&)      = delete;   // move assignment

    private:
        RegistryFileFormatPrivate* const d;
    };
}

#include "registryfileformat.inl"

#endif // GPUI_REGISTRYFILEFORMAT_H
