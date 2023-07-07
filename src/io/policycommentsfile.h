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

#ifndef GPUI_POLICYCOMMENTSFILE_H
#define GPUI_POLICYCOMMENTSFILE_H

#include "io.h"

#include "genericfile.h"
#include "policyfileformat.h"

#include <memory>

namespace comments {
    class PolicyComments;
}

namespace io {
    class PolicyCommentsFilePrivate;

    class GPUI_IO_EXPORT PolicyCommentsFile : public GenericFile<PolicyCommentsFile, PolicyFileFormat<PolicyCommentsFile>>
    {
    public:
        PolicyCommentsFile();

        ~PolicyCommentsFile();

        void add(std::shared_ptr<comments::PolicyComments> policyComments);

        void remove(std::shared_ptr<comments::PolicyComments> policyComments);

        std::vector<std::shared_ptr<comments::PolicyComments>> getAllPolicyComments();

        size_t policyCommentsCount();

        std::shared_ptr<comments::PolicyComments> getPolicyComments(const size_t index);

    private:
        PolicyCommentsFile(const PolicyCommentsFile&)            = delete;   // copy ctor
        PolicyCommentsFile(PolicyCommentsFile&&)                 = delete;   // move ctor
        PolicyCommentsFile& operator=(const PolicyCommentsFile&) = delete;   // copy assignment
        PolicyCommentsFile& operator=(PolicyCommentsFile&&)      = delete;   // move assignment

    private:
        PolicyCommentsFilePrivate* const d;
    };
}

#endif // GPUI_POLICYCOMMENTSFILE_H
