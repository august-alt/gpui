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

using namespace model::admx;

namespace io {

class PolicyFilePrivate
{
public:

};

PolicyFile::PolicyFile()
    : GenericFile<PolicyFile, PolicyFileFormat>()
    , d(new PolicyFilePrivate())
{

}

PolicyFile::~PolicyFile()
{

}

void PolicyFile::addPolicy(std::shared_ptr<Policy> policy)
{
    Q_UNUSED(policy);
}

void PolicyFile::removePolicy(std::shared_ptr<Policy> policy)
{
    Q_UNUSED(policy);
}

std::vector<std::shared_ptr<Policy>> PolicyFile::getPolicies()
{
    return std::vector<std::shared_ptr<Policy>>();
}

size_t PolicyFile::policyCount()
{
    return 0;
}

std::shared_ptr<Policy> PolicyFile::getPolicy(const size_t index)
{
    Q_UNUSED(index);
    return std::shared_ptr<Policy>(nullptr);
}

std::shared_ptr<Policy> PolicyFile::getPolicy(const std::string& name)
{
    Q_UNUSED(name);
    return std::shared_ptr<Policy>(nullptr);
}

bool PolicyFile::contains()
{
    return false;
}

}
