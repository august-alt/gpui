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
#include "cmtxformat.h"

#include "schema/cmtx.h"

#include "../comments/policycomments.h"

#include "../common/exceptionhandler.h"

namespace gpui
{
class XsdCommentsAdapter : public comments::PolicyComments
{
private:
    typedef ::GroupPolicy::CommentDefinitions::PolicyComments PolicyComments;

public:
    XsdCommentsAdapter(const PolicyComments &commentsResources)
        : comments::PolicyComments()
    {
        this->revision = commentsResources.revision();
        this->schemaVersion = commentsResources.schemaVersion();
        this->resources = std::make_unique<comments::LocalizationResourceReference>();

        for (const auto& comment : commentsResources.comments().admTemplate().comment())
        {
            comments::Comment currentCommnent;

            currentCommnent.policyRef = comment.policyRef();
            currentCommnent.commentText = comment.commentText();

            this->comments.push_back(currentCommnent);
        }
    }

    static std::unique_ptr<comments::PolicyComments> create(const PolicyComments &comments)
    {
        return std::make_unique<XsdCommentsAdapter>(comments);
    }
};


CmtxFormat::CmtxFormat()
    : io::PolicyFileFormat<io::PolicyResourcesFile>("cmtx")
{

}

bool CmtxFormat::read(std::istream &input, io::PolicyResourcesFile *file)
{
    auto operation = [&]() {
        std::unique_ptr<::GroupPolicy::CommentDefinitions::PolicyComments> policyComments
            = ::GroupPolicy::CommentDefinitions::policyComments(input,
                                                                ::xsd::cxx::tree::flags::dont_validate
                                                                | ::xsd::cxx::tree::flags::keep_dom
                                                                | ::xsd::cxx::tree::flags::own_dom);

//        file->add(XsdCommentsAdapter::create(*policyComments));
    };

    auto errorHandler = [&](const std::string &error) { this->setErrorString(error); };

    return ExceptionHandler::handleOperation(operation, errorHandler);
}

bool CmtxFormat::write(std::ostream &output, io::PolicyResourcesFile *file)
{
    (void)(output);
    (void)(file);

    return false;
}

}
