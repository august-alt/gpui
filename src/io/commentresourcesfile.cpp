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

#include "commentresourcesfile.h"

using namespace comments;

namespace io
{
class CommentResourcesFilePrivate
{
public:
    std::vector<std::shared_ptr<CommentDefinitionResources>> commentResources = {};
};

/*!
 * \class CommentResourcesFile CommentResourcesFile.h
 * \brief The CommentResourcesFile class represents a list of comment translations.
 * \ingroup io
 *
 * The PolicyResourcesFile class provides a container for comment resources.
 */
CommentResourcesFile::CommentResourcesFile()
    : GenericFile<CommentResourcesFile, PolicyFileFormat<CommentResourcesFile>>()
    , d(new CommentResourcesFilePrivate())
{

}

CommentResourcesFile::~CommentResourcesFile()
{
    delete d;
}

/*!
 * \brief CommentResourcesFile::addPolicyCommentResources Adds comment resources to the file.
 * \param policyComments Policy comment to add.
 */
void CommentResourcesFile::addPolicyCommentResources(std::shared_ptr<CommentDefinitionResources> policyComments)
{
    d->commentResources.emplace_back(policyComments);
}

/*!
 * \brief CommentResourcesFile::removePolicyCommentResources Removes comment resources from the file.
 * \param policyDefinitions  Policy comment to remove.
 */
void CommentResourcesFile::removePolicyCommentResources(std::shared_ptr<CommentDefinitionResources> policyComments)
{
    d->commentResources.erase(std::remove_if(d->commentResources.begin(),
                                              d->commentResources.end(),
                                              [policyComments](std::shared_ptr<CommentDefinitionResources> currentPolicy)
                                              {
                                                  return currentPolicy == policyComments;
                                              }));
}

/*!
 * \brief CommentResourcesFile::getAllPolicyCommentResources Returns all comment resources from the file.
 * \return All comment resources from the file.
 */
std::vector<std::shared_ptr<comments::CommentDefinitionResources> > CommentResourcesFile::getAllPolicyCommentResources()
{
    return d->commentResources;
}

/*!
 * \brief CommentResourcesFile::commentResourcesCount Return number of comment resources in current file.
 * \return Amount of comment resources in current file.
 */
size_t CommentResourcesFile::commentResourcesCount()
{
    return d->commentResources.size();
}

/*!
 * \brief CommentResourcesFile::getCommentResources Returns a comment resources by index.
 * \param index Index of a comment resources.
 * \return If comment resourcess are found than returns comment resourcess otherwise retruns null pointer.
 */
std::shared_ptr<comments::CommentDefinitionResources> CommentResourcesFile::getCommentResources(const size_t index)
{
    if (index < d->commentResources.size())
    {
        return d->commentResources[index];
    }

    return std::shared_ptr<CommentDefinitionResources>(nullptr);
}

} // namespace io
