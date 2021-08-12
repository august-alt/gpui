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
#include "../admx/registryvalue.h"

#include <algorithm>

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
    if (d->policy.enabledValue)
    {
        if (d->source.isValuePresent(d->policy.key, d->policy.valueName))
        {
            return checkValueState(d->policy.key, d->policy.valueName, d->policy.enabledValue.get());
        }
    }

    if (d->policy.elements.size() > 0)
    {
        for (const auto& element : d->policy.elements)
        {
            auto key = element->key.size() > 0
                    ? element->key
                    : d->policy.key;

            if (d->source.isValuePresent(key, element->valueName))
            {
                return true;
            }
        }
    }

    size_t enabledKeys = 0;
    for (const auto& listEntry : d->policy.enabledList)
    {
        if (d->source.isValuePresent(d->policy.key, listEntry->valueName))
        {
            if (checkValueState(d->policy.key, listEntry->valueName, listEntry->value.get()))
            {
                enabledKeys++;
            }
        }
    }

    return enabledKeys > 0 && enabledKeys == d->policy.enabledList.size();
}

bool PolicyStateManager::determineIfPolicyDisabled() const
{
    if (d->policy.disabledValue)
    {
        if (d->source.isValuePresent(d->policy.key, d->policy.valueName))
        {
            return checkValueState(d->policy.key, d->policy.valueName, d->policy.disabledValue.get());
        }
    }

    size_t disabledKeys = 0;
    for (const auto& listEntry : d->policy.disabledList)
    {
        if (d->source.isValuePresent(d->policy.key, listEntry->valueName))
        {
            if (checkValueState(d->policy.key, listEntry->valueName, listEntry->value.get()))
            {
                disabledKeys++;
            }
        }
    }

    return disabledKeys > 0 && disabledKeys == d->policy.disabledList.size();
}

void PolicyStateManager::setPolicyStateEnabled()
{
    if (d->policy.enabledValue)
    {
        setValueState(d->policy.key, d->policy.valueName, d->policy.enabledValue.get());
    }

    if (d->policy.enabledList.size() > 0)
    {
        for (const auto& listEntry : d->policy.enabledList)
        {
            setValueState(d->policy.key, listEntry->valueName, listEntry->value.get());
        }
    }

    for (const auto& element : d->policy.elements)
    {
        auto key = element->key.size() > 0
                ? element->key
                : d->policy.key;

        if (!d->source.isValuePresent(key, element->valueName))
        {
            // TODO: Implement.
        }
    }
}

void PolicyStateManager::setPolicyStateDisabled()
{
    if (d->policy.disabledValue)
    {
       setValueState(d->policy.key, d->policy.valueName, d->policy.disabledValue.get());
    }

    if (d->policy.disabledList.size() > 0)
    {
        for (const auto& listEntry : d->policy.disabledList)
        {
            setValueState(d->policy.key, listEntry->valueName, listEntry->value.get());
        }
    }
}

bool PolicyStateManager::checkValueState(const std::string &key, const std::string &valueName,
                                         model::admx::AbstractRegistryValue* registryValue) const
{
    QVariant value = d->source.getValue(key, valueName);
    switch (registryValue->type) {
    case model::admx::RegistryValueType::DECIMAL:
        return value.value<uint32_t>() == static_cast<model::admx::RegistryValue<uint32_t>*>(registryValue)->value;
        break;
    case model::admx::RegistryValueType::LONG_DECIMAL:
        return value.value<uint64_t>() == static_cast<model::admx::RegistryValue<uint64_t>*>(registryValue)->value;
        break;
    case model::admx::RegistryValueType::STRING: {
            auto v1 = value.value<std::vector<char>>();
            auto v2 = static_cast<model::admx::RegistryValue<std::vector<char>>*>(registryValue)->value;
            return std::equal(v1.begin(), v1.end(), v2.begin(), v2.end());
        } break;
    case model::admx::RegistryValueType::DELETE:
        return d->source.isValueMarkedForDeletion(key, valueName);
        break;
    default:
        break;
    }

    return false;
}

void PolicyStateManager::setValueState(const std::string &key, const std::string &valueName,
                                       admx::AbstractRegistryValue *registryValue)
{
    switch (registryValue->type) {
    case model::admx::RegistryValueType::DECIMAL:
        d->source.setValue(key, valueName, RegistryEntryType::REG_DWORD,
                           static_cast<model::admx::RegistryValue<uint32_t>*>(registryValue)->value);
        break;
    case model::admx::RegistryValueType::LONG_DECIMAL:
        d->source.setValue(key, valueName, RegistryEntryType::REG_QWORD,
                           QVariant::fromValue(static_cast<model::admx::RegistryValue<uint64_t>*>(registryValue)->value));
        break;
    case model::admx::RegistryValueType::STRING:
        d->source.setValue(key, valueName, RegistryEntryType::REG_SZ,
                           QVariant::fromValue(static_cast<model::admx::RegistryValue<std::vector<char> >*>(registryValue)->value));
        break;
    case model::admx::RegistryValueType::DELETE:
        d->source.markValueForDeletion(key, valueName);
        break;
    default:
        break;
    }
}

}

}
