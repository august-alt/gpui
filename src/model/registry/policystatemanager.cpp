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

#include "policystatemanager.h"

#include "abstractregistrysource.h"

#include "../admx/policy.h"
#include "../admx/policyelement.h"

#include <QObject>

namespace model {

namespace registry {

class PolicyStateManagerPrivate
{
public:
    AbstractRegistrySource &source;
    const admx::Policy &policy;

    PolicyStateManagerPrivate(AbstractRegistrySource &source, const admx::Policy &policy)
        : source(source)
        , policy(policy)
    {}
};

PolicyStateManager::PolicyStateManager(AbstractRegistrySource &source, const admx::Policy &policy)
    : d(new PolicyStateManagerPrivate(source, policy))
{
}

PolicyStateManager::~PolicyStateManager()
{
    delete d;
}

PolicyStateManager::PolicyState PolicyStateManager::determinePolicyState() const
{
    if (determineIfPolicyEnabled())
    {
        return STATE_ENABLED;
    }

    if (determineIfPolicyDisabled())
    {
        return STATE_DISABLED;
    }

    return STATE_NOT_CONFIGURED;
}

void PolicyStateManager::setupPolicyState(PolicyStateManager::PolicyState state)
{
    switch (state) {
    case STATE_ENABLED:
        setPolicyStateEnabled();
        break;
    case STATE_DISABLED:
        setPolicyStateDisabled();
        break;
    default:
        break;
    }
}

bool PolicyStateManager::determineIfPolicyEnabled() const
{
    if (d->policy.valueName.size() != 0)
    {
        if (d->source.isValuePresent(d->policy.key, d->policy.valueName))
        {
            return true;
        }
    }

    if (d->policy.elements.size() > 0)
    {
        for (const auto& element : d->policy.elements)
        {
            if (d->source.isValuePresent(element->key, element->valueName))
            {
                return true;
            }
        }
    }


    // TODO: Implement enabled list.

    return false;
}

bool PolicyStateManager::determineIfPolicyDisabled() const
{
    if (d->policy.valueName.size() != 0)
    {
        if (d->source.isValuePresent(d->policy.key, "disabled;" + d->policy.valueName))
        {
            return true;
        }
    }

    // TODO: Implement disabled list.

    return false;
}

void PolicyStateManager::setPolicyStateEnabled()
{
    if (d->policy.enabledValue)
    {
        // TODO: Implement enabled value.
    }

    if (d->policy.enabledList.size() > 0)
    {
        // TODO: Implement enabled list.
    }
}

void PolicyStateManager::setPolicyStateDisabled()
{
    if (d->policy.disabledValue)
    {
        // TODO: Implement enabled value.
    }

    if (d->policy.disabledList.size() > 0)
    {
        // TODO: Implement disabled list.
    }
}

}

}
