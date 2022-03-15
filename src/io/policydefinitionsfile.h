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

#ifndef GPUI_POLICYDEFINITIONSFILE_H
#define GPUI_POLICYDEFINITIONSFILE_H

#include "io.h"

#include "genericfile.h"
#include "policyfileformat.h"

#include <memory>

namespace model {
    namespace admx {
        class PolicyDefinitions;
    }
}

namespace io {
    class PolicyDefinitionsFilePrivate;

    class GPUI_IO_EXPORT PolicyDefinitionsFile : public GenericFile<PolicyDefinitionsFile, PolicyFileFormat<PolicyDefinitionsFile>>
    {
    public:
        PolicyDefinitionsFile();

        ~PolicyDefinitionsFile();

        void addPolicyDefinitions(std::shared_ptr<model::admx::PolicyDefinitions> policyDefinitions);

        void removePolicyDefinitions(std::shared_ptr<model::admx::PolicyDefinitions> policyDefinitions);

        std::vector<std::shared_ptr<model::admx::PolicyDefinitions>> getAllPolicyDefinitions();

        size_t policyDefinitionsCount();

        std::shared_ptr<model::admx::PolicyDefinitions> getPolicyDefinitions(const size_t index);

    private:
        PolicyDefinitionsFile(const PolicyDefinitionsFile&)            = delete;   // copy ctor
        PolicyDefinitionsFile(PolicyDefinitionsFile&&)                 = delete;   // move ctor
        PolicyDefinitionsFile& operator=(const PolicyDefinitionsFile&) = delete;   // copy assignment
        PolicyDefinitionsFile& operator=(PolicyDefinitionsFile&&)      = delete;   // move assignment

    private:
        PolicyDefinitionsFilePrivate* const d;
    };
}

#endif // GPUI_POLICYDEFINITIONSFILE_H
