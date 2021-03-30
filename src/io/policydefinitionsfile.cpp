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

#include "policydefinitionsfile.h"

#include "../model/admx/policydefinitions.h"

#include <algorithm>

using namespace model::admx;

namespace io {

class PolicyDefinitionsFilePrivate
{
public:
    std::vector<std::shared_ptr<PolicyDefinitions>> policyDefinitions;
};

/*!
 * \class PolicyDefinitionsFile PolicyDefinitionsFile.h
 * \brief The PolicyDefinitionsFile class represents a policy file.
 * \ingroup io
 *
 * The PolicyDefinitionsFile class provides a container for policy definitions.
 */

PolicyDefinitionsFile::PolicyDefinitionsFile()
    : GenericFile<PolicyDefinitionsFile, PolicyFileFormat<PolicyDefinitionsFile>>()
    , d(new PolicyDefinitionsFilePrivate())
{

}

PolicyDefinitionsFile::~PolicyDefinitionsFile()
{
    delete d;
}

/*!
 * \brief PolicyDefinitionsFile::addPolicyDefinitions Adds policy definitions to the file.
 * \param policyDefinitions Policy definitions to add.
 */
void PolicyDefinitionsFile::addPolicyDefinitions(std::shared_ptr<PolicyDefinitions> policyDefinitions)
{
    d->policyDefinitions.emplace_back(policyDefinitions);
}

/*!
 * \brief PolicyDefinitionsFile::removePolicyDefinitions Removes policy definitions from the file.
 * \param policyDefinitions Policy definitions to remove.
 */
void PolicyDefinitionsFile::removePolicyDefinitions(std::shared_ptr<PolicyDefinitions> policyDefinitions)
{
    d->policyDefinitions.erase(std::remove_if(d->policyDefinitions.begin(), d->policyDefinitions.end(),
                                     [policyDefinitions](std::shared_ptr<PolicyDefinitions> currentPolicy)
                                     {
                                           return currentPolicy == policyDefinitions;
                                     }));
}

/*!
 * \brief PolicyDefinitionsFile::getAllPolicyDefinitions Returns all policy definitions from the file.
 * \return
 */
std::vector<std::shared_ptr<PolicyDefinitions> > PolicyDefinitionsFile::getAllPolicyDefinitions()
{
    return d->policyDefinitions;
}

/*!
 * \brief PolicyDefinitionsFile::policyDefinitionsCount Return number of policy definitions in current file.
 * \return Amount of policy definitions in current file.
 */
size_t PolicyDefinitionsFile::policyDefinitionsCount()
{
    return d->policyDefinitions.size();
}

/*!
 * \brief PolicyDefinitionsFile::getPolicyDefinitions Returns a policy definition by index.
 * \param index Index of a policy definitions.
 * \return If policy definitions are found than returns policy definitions otherwise retruns null pointer.
 */
std::shared_ptr<PolicyDefinitions> PolicyDefinitionsFile::getPolicyDefinitions(const size_t index)
{
    if (index < d->policyDefinitions.size()) {
        return d->policyDefinitions[index];
    }

    return std::shared_ptr<PolicyDefinitions>(nullptr);
}

}
