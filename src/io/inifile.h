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

#ifndef GPUI_INIFILE_H
#define GPUI_INIFILE_H

#include "io.h"

#include "genericfile.h"
#include "policyfileformat.h"

#include <QMultiMap>
#include <QMap>

#include <memory>
#include <string>

namespace io {
    class IniFilePrivate;

    class GPUI_IO_EXPORT IniFile : public GenericFile<IniFile, PolicyFileFormat<IniFile>>
    {
    public:
        typedef QMultiMap<std::string, std::string> section;
        typedef QMap<std::string, section>  sections;

    public:
        IniFile();

        ~IniFile();

        void addValue(const std::string& section, const std::string& key, const std::string& value);

        void removeKey(const std::string& section, const std::string& key);

        std::shared_ptr<sections> getAllSections();

    private:
        IniFile(const IniFile&)            = delete;   // copy ctor
        IniFile(IniFile&&)                 = delete;   // move ctor
        IniFile& operator=(const IniFile&) = delete;   // copy assignment
        IniFile& operator=(IniFile&&)      = delete;   // move assignment

    private:
        IniFilePrivate* const d;
    };
}

#endif // GPUI_INIFILE_H
