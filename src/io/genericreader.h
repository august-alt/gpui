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

#ifndef GPUI_GENERIC_READER_H
#define GPUI_GENERIC_READER_H

#include "io.h"

#include <memory>
#include <string>

namespace io {
    class GPUI_IO_EXPORT GenericReader
    {
    public:
        virtual ~GenericReader() = default;

        template<typename TData, typename TFormat>
        std::unique_ptr<TData> load(const std::string& fileName);
    };
}

#include "genericreader.inl"

#endif // GPUI_GENERIC_READER_H
