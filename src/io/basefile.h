/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_BASEFILE_H
#define GPUI_BASEFILE_H

#include "io.h"

#include "genericfile.h"
#include "policyfileformat.h"

#include <memory>

namespace io {
    template <typename TContent>
    class BaseFilePrivate;

    template <typename TContent>
    class GPUI_IO_EXPORT BaseFile : public GenericFile<BaseFile<TContent>, PolicyFileFormat<BaseFile<TContent>>>
    {
    public:
        BaseFile();

        ~BaseFile();

        void add(std::shared_ptr<TContent> resources);

        void remove(std::shared_ptr<TContent> resources);

        std::vector<std::shared_ptr<TContent>> getAll();

        size_t count();

        std::shared_ptr<TContent> get(const size_t index);

    private:
        BaseFile(const BaseFile&)            = delete;   // copy ctor
        BaseFile(BaseFile&&)                 = delete;   // move ctor
        BaseFile& operator=(const BaseFile&) = delete;   // copy assignment
        BaseFile& operator=(BaseFile&&)      = delete;   // move assignment

    private:
        BaseFilePrivate<TContent>* const d;
    };
}

#include "basefile.inl"

#endif // GPUI_BASEFILE_H
