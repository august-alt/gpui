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

#include "policyfile.h"

#include "../model/admx/policy.h"

#include <algorithm>

using namespace model::admx;

namespace io {

class PolicyFilePrivate
{
public:
    std::vector<std::shared_ptr<Policy>> policies;
};

/*!
 * \class PolicyFile policyfile.h
 * \brief The PolicyFile class represents a policy file.
 * \ingroup io
 *
 * The PolicyFile class provides a container for policies.
 */

PolicyFile::PolicyFile()
    : GenericFile<PolicyFile, PolicyFileFormat>()
    , d(new PolicyFilePrivate())
{

}

/*!
 * \brief PolicyFile::addPolicy Adds policy to the file.
 * \param policy Policy to add.
 */
void PolicyFile::addPolicy(std::shared_ptr<Policy> policy)
{
    d->policies.emplace_back(policy);
}

/*!
 * \brief PolicyFile::removePolicy Removes policy from the file.
 * \param policy Policy to remove.
 */
void PolicyFile::removePolicy(std::shared_ptr<Policy> policy)
{
    std::remove_if(d->policies.begin(), d->policies.end(),
                   [policy](std::shared_ptr<Policy> currentPolicy)
                   {
                        return currentPolicy == policy;
                   });
}

/*!
 * \brief PolicyFile::getPolicies Returns all policies from the file.
 * \return
 */
std::vector<std::shared_ptr<Policy>> PolicyFile::getPolicies()
{
    return d->policies;
}

/*!
 * \brief PolicyFile::policyCount Return number of policies in current file.
 * \return Amount of policies in current file.
 */
size_t PolicyFile::policyCount()
{
    return d->policies.size();
}

/*!
 * \brief PolicyFile::getPolicy Returns a policy by index.
 * \param index Index of a policy.
 * \return If policy is found than returns policy and if its not retruns null pointer.
 */
std::shared_ptr<Policy> PolicyFile::getPolicy(const size_t index)
{
    if (index < d->policies.size()) {
        return d->policies[index];
    }

    return std::shared_ptr<Policy>(nullptr);
}

/*!
 * \brief PolicyFile::getPolicy Returns a policy by it's name.
 * \param name Name of a policy.
 * \return If policy is found than returns pointer to the policy and otherwise returns null pointer.
 */
std::shared_ptr<Policy> PolicyFile::getPolicy(const std::string& name)
{
    for (const auto& policy : d->policies) {
        if (policy->name.compare(name) == 0) {
            return policy;
        }
    }

    return std::shared_ptr<Policy>(nullptr);
}

/*!
 * \brief PolicyFile::contains Checks if policy file contains a certain policy.
 * \param name Name of a policy to check.
 * \return Returns true if policy was found.
 */
bool PolicyFile::contains(const std::string& name)
{
    return std::any_of(d->policies.begin(), d->policies.end(),
                       [name](std::shared_ptr<Policy> policy)
                       {
                            return policy->name.compare(name) == 0;
                       });
}

}
