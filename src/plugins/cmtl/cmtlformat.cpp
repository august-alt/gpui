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
#include "cmtlformat.h"

#include "schema/cmtl.h"

#include "../comments/commentdefinitionresources.h"

#include "../common/exceptionhandler.h"

namespace gpui
{
class XsdCommentDefinitionResourcesAdapater : public comments::CommentDefinitionResources
{
private:
    typedef ::GroupPolicy::CommentDefinitions::CommentDefinitionResources CommentDefinitionResources;

public:
    XsdCommentDefinitionResourcesAdapater(const CommentDefinitionResources &comments)
        : comments::CommentDefinitionResources()
    {
        this->revision = comments.revision();
        this->schemaVersion = comments.schemaVersion();

        for (const auto& stringResource : comments.resources().stringTable().string())
        {
            this->stringTable.push_back(std::make_pair(stringResource.id(), stringResource));
        }
    }

    static std::unique_ptr<comments::CommentDefinitionResources> create(const CommentDefinitionResources &comments)
    {
        return std::make_unique<XsdCommentDefinitionResourcesAdapater>(comments);
    }
};

class CommentDefinitionResourcesXsdAdapter : public ::GroupPolicy::CommentDefinitions::CommentDefinitionResources
{
private:
    typedef ::comments::CommentDefinitionResources CommentDefinitionResources;

public:
    static std::unique_ptr<::GroupPolicy::CommentDefinitions::CommentDefinitionResources> create(const CommentDefinitionResources &comments)
    {
        ::GroupPolicy::CommentDefinitions::StringTable stringTable;

        for (const auto& stringResource : comments.stringTable)
        {
            ::GroupPolicy::CommentDefinitions::String string(stringResource.first, stringResource.second);

            stringTable.string().push_back(string);
        }

        ::GroupPolicy::CommentDefinitions::Resources resources(stringTable);

        return std::make_unique<::GroupPolicy::CommentDefinitions::CommentDefinitionResources>(resources,
                                                                                               comments.revision,
                                                                                               comments.schemaVersion);
    }
};

CmtlFormat::CmtlFormat()
    : io::PolicyFileFormat<io::CommentResourcesFile>("cmtl")
{

}

bool CmtlFormat::read(std::istream &input, io::CommentResourcesFile *file)
{
    auto operation = [&]() {
        std::unique_ptr<::GroupPolicy::CommentDefinitions::CommentDefinitionResources> policyComments
            = ::GroupPolicy::CommentDefinitions::commentDefinitionResources(input,
                                                                            ::xsd::cxx::tree::flags::dont_validate
                                                                            | ::xsd::cxx::tree::flags::keep_dom
                                                                            | ::xsd::cxx::tree::flags::own_dom);

        file->addPolicyCommentResources(XsdCommentDefinitionResourcesAdapater::create(*policyComments));
    };

    auto errorHandler = [&](const std::string &error) { this->setErrorString(error); };

    return ExceptionHandler::handleOperation(operation, errorHandler);
}

bool CmtlFormat::write(std::ostream &output, io::CommentResourcesFile *file)
{
    auto operation = [&]() -> void {
        std::vector<std::shared_ptr<comments::CommentDefinitionResources>> policyComments
            = file->getAllPolicyCommentResources();

        for (const auto& policyComment : policyComments)
        {
            std::unique_ptr<::GroupPolicy::CommentDefinitions::CommentDefinitionResources> outputComments
                = CommentDefinitionResourcesXsdAdapter::create(*policyComment);

            ::GroupPolicy::CommentDefinitions::commentDefinitionResources(output, *outputComments);
        }
    };

    auto errorHandler = [&](const std::string &error) { this->setErrorString(error); };

    return ExceptionHandler::handleOperation(operation, errorHandler);
}

}
