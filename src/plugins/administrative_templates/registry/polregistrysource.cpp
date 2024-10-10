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

#include "polregistrysource.h"

#include "registry.h"

#include "../../src/core/pluginstorage.h"

#include <QDebug>

namespace model
{
namespace registry
{
class PolRegistrySourcePrivate
{
public:
    std::string fileName{};
    std::shared_ptr<Registry> registry = nullptr;
};

PolRegistrySource::PolRegistrySource(std::shared_ptr<Registry> registry)
    : AbstractRegistrySource(POL_REGISTRY_TYPE)
    , d(new PolRegistrySourcePrivate())
{
    d->registry = registry;
}

PolRegistrySource::~PolRegistrySource()
{
    delete d;
}

QVariant PolRegistrySource::getValue(const std::string &key, const std::string &valueName) const
{
    for (const auto &entry : d->registry->registryEntries)
    {
        if (entry->key.compare(key.c_str()) == 0 && entry->value.compare(valueName.c_str()) == 0)
        {
            switch (entry->type)
            {
            case REG_SZ:
                return QVariant(static_cast<RegistryEntry<QString> *>(entry.get())->data);
            case REG_BINARY:
                return QVariant(static_cast<RegistryEntry<QString> *>(entry.get())->data);
            case REG_DWORD:
                return QVariant(static_cast<RegistryEntry<uint32_t> *>(entry.get())->data);
            case REG_DWORD_BIG_ENDIAN:
                return QVariant(static_cast<RegistryEntry<uint32_t> *>(entry.get())->data);
            case REG_EXPAND_SZ:
                return QVariant(static_cast<RegistryEntry<QString> *>(entry.get())->data);
            case REG_MULTI_SZ:
                return QVariant(static_cast<RegistryEntry<QStringList> *>(entry.get())->data);
            case REG_QWORD:
                return QVariant::fromValue(static_cast<RegistryEntry<uint64_t> *>(entry.get())->data);
            default:
                break;
            }
        }
    }

    return QVariant();
}

void PolRegistrySource::setValue(const std::string &key,
                                 const std::string &valueName,
                                 RegistryEntryType registryEntryType,
                                 const QVariant &data)
{
    qWarning() << "Set value" << key.c_str() << valueName.c_str() << data;

    if (isValuePresent(key, valueName))
    {
        switch (registryEntryType)
        {
        case REG_BINARY:
            updateValue(key, valueName, registryEntryType, data.value<QString>());
            break;
        case REG_DWORD:
            updateValue(key, valueName, registryEntryType, data.value<uint32_t>());
            break;
        case REG_DWORD_BIG_ENDIAN:
            updateValue(key, valueName, registryEntryType, data.value<uint32_t>());
            break;
        case REG_EXPAND_SZ:
            updateValue(key, valueName, registryEntryType, data.value<QString>());
            break;
        case REG_MULTI_SZ:
            updateValue(key, valueName, registryEntryType, data.value<QStringList>());
            break;
        case REG_QWORD:
            updateValue(key, valueName, registryEntryType, data.value<uint64_t>());
            break;
        case REG_SZ:
            updateValue(key, valueName, registryEntryType, data.value<QString>());
            break;
        default:
            break;
        }

        return;
    }

    createValue(key, valueName, registryEntryType, data);
}

bool PolRegistrySource::isValuePresent(const std::string &key, const std::string &valueName) const
{
    for (const auto &entry : d->registry->registryEntries)
    {
        if (entry->key.compare(key.c_str()) == 0 && entry->value.compare(valueName.c_str()) == 0)
        {
            return true;
        }
    }

    return false;
}

void PolRegistrySource::markValueForDeletion(const std::string &key, const std::string &valueName)
{
    if (valueName.compare(0, 6, "**del.") == 0)
    {
        return;
    }

    std::string deleteValueName = "**del." + valueName;

    clearValue(key, valueName);

    setValue(key, deleteValueName, RegistryEntryType::REG_SZ, QString(' '));
    qWarning() << "Marking value for deletion: " << key.c_str() << valueName.c_str();
}

bool PolRegistrySource::undeleteValue(const std::string &key, const std::string &valueName)
{
    std::string deleteValueName = "**del." + valueName;

    for (const auto &entry : d->registry->registryEntries)
    {
        if (entry->key.compare(key.c_str()) == 0 && entry->value.compare(deleteValueName.c_str()) == 0)
        {
            entry->value = QString::fromStdString(valueName);
        }
    }

    return false;
}

bool PolRegistrySource::isValueMarkedForDeletion(const std::string &key, const std::string &valueName) const
{
    if (isValuePresent(key, "**del." + valueName))
    {
        return true;
    }

    if (isValuePresent(key, "**delvals."))
    {
        return true;
    }

    if (isValuePresent(key, "**deletevalues"))
    {
        auto valueList = getValue(key, "**deletevalues").toString().split(';');
        for (const auto &value : valueList)
        {
            if (value.compare(valueName.c_str()) == 0)
            {
                return true;
            }
        }
    }

    return false;
}

std::vector<std::string> PolRegistrySource::getValueNames(const std::string &key) const
{
    std::vector<std::string> result;

    for (const auto &entry : d->registry->registryEntries)
    {
        if (entry->key.compare(key.c_str()) == 0)
        {
            result.push_back(entry->value.toStdString());
        }
    }

    return result;
}

static bool isSpecialValueName(const std::string &str)
{
    // TODO: make case-insensitive
    // TODO: check for others
    return (str == "**deletevalues") || 
           (str == "**delvals.")     ||
           (str == "**deletekeys")   ||
           (str == "**securekey")    || 
           (str.length() >= 6 && strncmp(str.c_str(), "**del.", 6) == 0) ||
           (str.length() >= 7 && strncmp(str.c_str(), "**soft.", 7) == 0);
}

std::vector<std::string> PolRegistrySource::getNonSpecialValueNames(const std::string &key) const 
{
    std::vector<std::string> result = getValueNames(key);

    result.erase(std::remove_if(result.begin(), 
                                result.end(), 
                                &isSpecialValueName), 
                 result.end());
    return result;
}

void PolRegistrySource::markKeyForDeletion(const std::string &key)
{
    std::vector<std::string> values = getValueNames(key);
    for (const auto &value : values)
    {
        markValueForDeletion(key, value);
    }
}

void PolRegistrySource::clearKey(const std::string &key)
{
    std::vector<std::string> values = getValueNames(key);
    for (const auto &value : values)
    {
        clearValue(key, value);
    }
}

void PolRegistrySource::clearValue(const std::string &key, const std::string &valueName)
{
    auto &entries = d->registry->registryEntries;

    std::string delValue("**del." + valueName);
    std::string delVals("**delvals." + valueName);

    auto entryMatch = [&key, &valueName, &delValue, &delVals](const auto &entry) {
        bool checkValue = (entry->value.compare(valueName.c_str()) == 0
                           || entry->value.compare(delValue.c_str()) == 0
                           || entry->value.compare(delVals.c_str()) == 0);

        return entry->key.compare(key.c_str()) == 0 && checkValue;
    };

    entries.erase(std::remove_if(entries.begin(), entries.end(), entryMatch), entries.end());
}

bool PolRegistrySource::ifValueStartsWith(const std::string &key, const std::string &valueStart) const
{
    for (const auto &entry : d->registry->registryEntries)
    {
        if (entry->key.compare(key.c_str()) == 0 && entry->value.startsWith(valueStart.c_str()) == 0)
        {
            return true;
        }
    }

    return false;
}

template<typename T>
void PolRegistrySource::updateValue(const std::string &key,
                                    const std::string &valueName,
                                    RegistryEntryType registryEntryType,
                                    const T &data)
{
    for (const auto &entry : d->registry->registryEntries)
    {
        if (entry->key.compare(key.c_str()) == 0 && entry->value.compare(valueName.c_str()) == 0)
        {
            if (entry->type == registryEntryType)
            {
                static_cast<RegistryEntry<T> *>(entry.get())->data = data;
            }
            else
            {
                qWarning() << "Attempt to assign value of different type to: " << key.c_str() << valueName.c_str();
            }
        }
    }
}

template<typename T>
void PolRegistrySource::createValue(const std::string &key,
                                    const std::string &valueName,
                                    RegistryEntryType registryEntryType,
                                    const T &data)
{
    auto entry   = std::make_unique<RegistryEntry<T>>();
    entry->key   = key.c_str();
    entry->value = valueName.c_str();
    entry->type  = registryEntryType;
    entry->data  = data;

    d->registry->registryEntries.push_back(std::move(entry));
}

} // namespace registry

} // namespace model
