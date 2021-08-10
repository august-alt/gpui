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

#include "../pluginstorage.h"

namespace model {

namespace registry {

class PolRegistrySourcePrivate
{
public:
    std::string fileName;
    Registry registry;
};

PolRegistrySource::PolRegistrySource(const std::string& fileName)
    : AbstractRegistrySource(POL_REGISTRY_TYPE)
    , d(new PolRegistrySourcePrivate())
{
    d->fileName = fileName;
}

PolRegistrySource::PolRegistrySource(std::shared_ptr<Registry> registry)
    : AbstractRegistrySource(POL_REGISTRY_TYPE)
    , d(new PolRegistrySourcePrivate())
{
    for (auto& entry : registry->registryEntries)
    {
        d->registry.registryEntries.push_back(std::move(entry));
    }
}

PolRegistrySource::~PolRegistrySource()
{
    delete d;
}

QVariant PolRegistrySource::getValue(const std::string &key, const std::string &valueName) const
{
    for (const auto& entry : d->registry.registryEntries)
    {
        if (entry->key.compare(key.c_str()) == 0 && entry->value.compare(valueName.c_str()) == 0)
        {
            switch (entry->type) {
            case REG_SZ:
                return QVariant(static_cast<RegistryEntry<char*>*>(entry.get())->data);
            case REG_BINARY:
                return QVariant(static_cast<RegistryEntry<char*>*>(entry.get())->data);
            case REG_DWORD:
                return QVariant(static_cast<RegistryEntry<uint32_t>*>(entry.get())->data);
            case REG_DWORD_BIG_ENDIAN:
                return QVariant(static_cast<RegistryEntry<uint32_t>*>(entry.get())->data);
            case REG_EXPAND_SZ:
                return QVariant(static_cast<RegistryEntry<char*>*>(entry.get())->data);
            case REG_MULTI_SZ:
                return QVariant(static_cast<RegistryEntry<char*>*>(entry.get())->data);
            case REG_QWORD:
            default:
                break;
            }
        }
    }

    return QVariant();
}

void PolRegistrySource::setValue(const std::string &key, const std::string &valueName, RegistryEntryType type, const QVariant &data)
{
    if (isValuePresent(key, valueName))
    {
        switch (type) {
        case REG_BINARY:
            updateValue(key, valueName, data.value<std::vector<char>>());
            break;
        case REG_DWORD:
            updateValue(key, valueName, data.value<uint32_t>());
            break;
        case REG_DWORD_BIG_ENDIAN:
            updateValue(key, valueName, data.value<uint32_t>());
            break;
        case REG_EXPAND_SZ:
            updateValue(key, valueName, data.value<std::vector<char>>());
            break;
        case REG_MULTI_SZ:
            updateValue(key, valueName, data.value<std::vector<char>>());
            break;
        case REG_QWORD:
            updateValue(key, valueName, data.value<uint64_t>());
            break;
        case REG_SZ:
            updateValue(key, valueName, data.value<std::vector<char>>());
            break;
        default:
            break;
        }
    }
}

bool PolRegistrySource::isValuePresent(const std::string &key, const std::string &valueName) const
{
    for (const auto& entry : d->registry.registryEntries)
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
    Q_UNUSED(key);
    Q_UNUSED(valueName);
}

void PolRegistrySource::undeleteValue(const std::string &key, const std::string &valueName)
{
    Q_UNUSED(key);
    Q_UNUSED(valueName);
}

std::vector<std::string> PolRegistrySource::getValueNames(const std::string &key) const
{
    std::vector<std::string> result;

    for (const auto& entry : d->registry.registryEntries)
    {
        if (entry->key.compare(key.c_str()) == 0)
        {
            result.push_back(entry->value.toStdString());
        }
    }

    return result;
}

void PolRegistrySource::clearKey(const std::string &key)
{
    std::vector<std::string> values = getValueNames(key);
    for (const auto& value : values)
    {
        markValueForDeletion(key, value);
    }
}

bool PolRegistrySource::read()
{
    return false;
}

bool PolRegistrySource::write()
{
    return false;
}

template<typename T>
void PolRegistrySource::updateValue(const std::string &key, const std::string &valueName, const T &data)
{
    for (const auto& entry : d->registry.registryEntries)
    {
        if (entry->key.compare(key.c_str()) == 0 && entry->value.compare(valueName.c_str()) == 0)
        {
            static_cast<RegistryEntry<T>*>(entry.get())->data = data;
        }
    }
}

}

}
