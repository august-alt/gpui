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

#include "policyresourcesfile.h"

#include "../model/presentation/policyresources.h"

#include <algorithm>

using namespace model::presentation;

namespace io {

class PolicyResourcesFilePrivate
{
public:
    std::vector<std::shared_ptr<PolicyResources>> policyDefinitions;
};

/*!
 * \class PolicyResourcesFile PolicyResourcesFile.h
 * \brief The PolicyResourcesFile class represents a policy file.
 * \ingroup io
 *
 * The PolicyResourcesFile class provides a container for policy definitions.
 */

PolicyResourcesFile::PolicyResourcesFile()
    : GenericFile<PolicyResourcesFile, PolicyFileFormat<PolicyResourcesFile>>()
    , d(new PolicyResourcesFilePrivate())
{

}

PolicyResourcesFile::~PolicyResourcesFile()
{
    delete d;
}

/*!
 * \brief PolicyResourcesFile::addPolicyDefinitions Adds policy definitions to the file.
 * \param policyDefinitions Policy definitions to add.
 */
void PolicyResourcesFile::add(std::shared_ptr<PolicyResources> policyDefinitions)
{
    d->policyDefinitions.emplace_back(policyDefinitions);
}

/*!
 * \brief PolicyResourcesFile::removePolicyDefinitions Removes policy definitions from the file.
 * \param policyDefinitions Policy definitions to remove.
 */
void PolicyResourcesFile::remove(std::shared_ptr<PolicyResources> policyDefinitions)
{
    d->policyDefinitions.erase(std::remove_if(d->policyDefinitions.begin(), d->policyDefinitions.end(),
                                     [policyDefinitions](std::shared_ptr<PolicyResources> currentPolicy)
                                     {
                                           return currentPolicy == policyDefinitions;
                                     }));
}

/*!
 * \brief PolicyResourcesFile::getAllPolicyDefinitions Returns all policy definitions from the file.
 * \return
 */
std::vector<std::shared_ptr<PolicyResources> > PolicyResourcesFile::getAll()
{
    return d->policyDefinitions;
}

/*!
 * \brief PolicyResourcesFile::policyDefinitionsCount Return number of policy definitions in current file.
 * \return Amount of policy definitions in current file.
 */
size_t PolicyResourcesFile::count()
{
    return d->policyDefinitions.size();
}

/*!
 * \brief PolicyResourcesFile::getPolicyDefinitions Returns a policy definition by index.
 * \param index Index of a policy definitions.
 * \return If policy definitions are found than returns policy definitions otherwise retruns null pointer.
 */
std::shared_ptr<PolicyResources> PolicyResourcesFile::get(const size_t index)
{
    if (index < d->policyDefinitions.size()) {
        return d->policyDefinitions[index];
    }

    return std::shared_ptr<PolicyResources>(nullptr);
}

}
