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

#ifndef GPUI_POLICYRESOURCESFILE_H
#define GPUI_POLICYRESOURCESFILE_H

#include "io.h"

#include "genericfile.h"
#include "policyfileformat.h"

#include <memory>

namespace model {
    namespace presentation {
        class PolicyResources;
    }
}

namespace io {
    class PolicyResourcesFilePrivate;

    class GPUI_IO_EXPORT PolicyResourcesFile : public GenericFile<PolicyResourcesFile, PolicyFileFormat<PolicyResourcesFile>>
    {
    public:
        PolicyResourcesFile();

        ~PolicyResourcesFile();

        void add(std::shared_ptr<model::presentation::PolicyResources> resources);

        void remove(std::shared_ptr<model::presentation::PolicyResources> resources);

        std::vector<std::shared_ptr<model::presentation::PolicyResources>> getAll();

        size_t count();

        std::shared_ptr<model::presentation::PolicyResources> get(const size_t index);

    private:
        PolicyResourcesFilePrivate* const d;
    };
}

#endif // GPUI_POLICYRESOURCESFILE_H
