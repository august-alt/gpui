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

#ifndef GPUI_RegistryFile_H
#define GPUI_RegistryFile_H

#include "io.h"

#include "genericfile.h"
#include "registryfileformat.h"

#include <memory>

namespace model {
    namespace registry {
        class Registry;
    }
}

namespace io {
    class RegistryFilePrivate;

    class GPUI_IO_EXPORT RegistryFile : public GenericFile<RegistryFile, RegistryFileFormat<RegistryFile>>
    {
    public:
        RegistryFile();

        ~RegistryFile();

        void setRegistry(std::shared_ptr<model::registry::Registry> registryEntry);

        void removeRegistry();

        std::shared_ptr<model::registry::Registry> getRegistry();

    private:
        RegistryFilePrivate* const d;
    };
}

#endif // GPUI_RegistryFile_H
