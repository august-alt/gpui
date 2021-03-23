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

#ifndef GPUI_POLICYFILE_H
#define GPUI_POLICYFILE_H

#include "io.h"

#include "genericfile.h"
#include "policyfileformat.h"

#include <memory>

namespace model {
    namespace admx {
        class Policy;
    }
}

namespace io {
    class PolicyFilePrivate;

    class GPUI_IO_EXPORT PolicyFile : public GenericFile<PolicyFile, PolicyFileFormat>
    {
    public:
        PolicyFile();

        ~PolicyFile();

        void addPolicy(std::shared_ptr<model::admx::Policy> policy);

        void removePolicy(std::shared_ptr<model::admx::Policy> policy);

        std::vector<std::shared_ptr<model::admx::Policy>> getPolicies();

        size_t policyCount();

        std::shared_ptr<model::admx::Policy> getPolicy(const size_t index);

        std::shared_ptr<model::admx::Policy> getPolicy(const std::string& name);

        bool contains();

    private:
        std::unique_ptr<PolicyFilePrivate> d;
    };
}

#endif // GPUI_POLICYFILE_H
