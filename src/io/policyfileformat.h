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

#ifndef GPUI_POLICYFILEFORMAT_H
#define GPUI_POLICYFILEFORMAT_H

#include "io.h"

#include <memory>
#include <string>
#include <vector>

namespace io {
    class PolicyFileFormatPrivate;

    template <typename TPolicyFile>
    class GPUI_IO_EXPORT PolicyFileFormat
    {
    public:
        std::string getName();

        virtual bool read(std::istream& input, TPolicyFile* file) = 0;

        virtual bool write(std::ostream& output, TPolicyFile* file) = 0;

        std::string getErrorString();

        static PolicyFileFormat* create(const std::string& name);

        static std::vector<std::string> getFormats();

        virtual ~PolicyFileFormat();

    protected:
        void setErrorString(const std::string& error);

        PolicyFileFormat(const std::string& name);

    private:
        PolicyFileFormatPrivate* const d;
    };
}

#include "policyfileformat.inl"

#endif // GPUI_POLICYFILEFORMAT_H
