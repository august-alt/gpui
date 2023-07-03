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

#include "policycommentsfile.h"

using namespace comments;

namespace io
{

class PolicyCommentsFilePrivate
{
public:
    std::vector<std::shared_ptr<PolicyComments>> policyComments = {};
};

/*!
 * \class PolicyCommentsFile PolicyCommentsFile.h
 * \brief The PolicyCommentsFile class represents a policy file.
 * \ingroup io
 *
 * The PolicyCommentsFile class provides a container for policy comments.
 */

PolicyCommentsFile::PolicyCommentsFile()
    : GenericFile<PolicyCommentsFile, PolicyFileFormat<PolicyCommentsFile>>()
    , d(new PolicyCommentsFilePrivate())
{}

PolicyCommentsFile::~PolicyCommentsFile()
{
    delete d;
}

/*!
 * \brief PolicyCommentsFile::addPolicyComments Adds policy comments to the file.
 * \param policyComments Policy comments to add.
 */
void PolicyCommentsFile::addPolicyComments(std::shared_ptr<PolicyComments> policyComments)
{
    d->policyComments.emplace_back(policyComments);
}

/*!
 * \brief PolicyCommentsFile::removePolicyComments Removes policy comments from the file.
 * \param policyComments Policy comments to remove.
 */
void PolicyCommentsFile::removePolicyComments(std::shared_ptr<PolicyComments> policyComments)
{
    d->policyComments.erase(std::remove_if(d->policyComments.begin(),
                                              d->policyComments.end(),
                                              [policyComments](std::shared_ptr<PolicyComments> currentPolicy) {
                                                  return currentPolicy == policyComments;
                                              }));
}

/*!
 * \brief PolicyCommentsFile::getAllPolicyComments Returns all policy comments from the file.
 * \return
 */
std::vector<std::shared_ptr<PolicyComments>> PolicyCommentsFile::getAllPolicyComments()
{
    return d->policyComments;
}

/*!
 * \brief PolicyCommentsFile::policyCommentsCount Return number of policy comments in current file.
 * \return Amount of policy comments in current file.
 */
size_t PolicyCommentsFile::policyCommentsCount()
{
    return d->policyComments.size();
}

/*!
 * \brief PolicyCommentsFile::getPolicyComments Returns a policy definition by index.
 * \param index Index of a policy comments.
 * \return If policy comments are found than returns policy comments otherwise retruns null pointer.
 */
std::shared_ptr<PolicyComments> PolicyCommentsFile::getPolicyComments(const size_t index)
{
    if (index < d->policyComments.size())
    {
        return d->policyComments[index];
    }

    return std::shared_ptr<PolicyComments>(nullptr);
}

}
