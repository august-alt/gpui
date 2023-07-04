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

#ifndef GPUI_COMMENTRESOURCESSFILE_H
#define GPUI_COMMENTRESOURCESSFILE_H

#include "io.h"

#include "genericfile.h"
#include "policyfileformat.h"

#include <memory>

namespace comments {
    class CommentDefinitionResources;
}

namespace io {
    class CommentResourcesFilePrivate;

    class GPUI_IO_EXPORT CommentResourcesFile : public GenericFile<CommentResourcesFile, PolicyFileFormat<CommentResourcesFile>>
    {
    public:
        CommentResourcesFile();

        ~CommentResourcesFile();

        void addPolicyCommentResources(std::shared_ptr<comments::CommentDefinitionResources> policyComments);

        void removePolicyCommentResources(std::shared_ptr<comments::CommentDefinitionResources> policyComments);

        std::vector<std::shared_ptr<comments::CommentDefinitionResources>> getAllPolicyCommentResources();

        size_t commentResourcesCount();

        std::shared_ptr<comments::CommentDefinitionResources> getCommentResources(const size_t index);

    private:
        CommentResourcesFile(const CommentResourcesFile&)            = delete;   // copy ctor
        CommentResourcesFile(CommentResourcesFile&&)                 = delete;   // move ctor
        CommentResourcesFile& operator=(const CommentResourcesFile&) = delete;   // copy assignment
        CommentResourcesFile& operator=(CommentResourcesFile&&)      = delete;   // move assignment

    private:
        CommentResourcesFilePrivate* const d;
    };
}

#endif // GPUI_COMMENTRESOURCESSFILE_H
