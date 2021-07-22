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

#include "polformat.h"

#include "pregdata.h"
#include "pregparser.h"

#include "../../../src/model/registry/registry.h"
#include "../../../src/model/registry/registryentry.h"

namespace gpui {

class RegistryEntryAdapter
{
private:
    static std::unique_ptr<model::registry::AbstractRegistryEntry>adaptCharEntry(const preg::PregEntry& entry,
                                                                             model::registry::RegistryEntryType type)
    {
        auto registryEntry = std::make_unique<model::registry::RegistryEntry<std::vector<char> > >();
        registryEntry->key = entry.key.c_str();
        registryEntry->type = type;
        registryEntry->value = entry.value.c_str();
        if (entry.data) {
            registryEntry->data = std::vector<char>(*entry.data);
            delete[] entry.data;
        }

        return registryEntry;
    }

    template<typename TData>
    static std::unique_ptr<model::registry::AbstractRegistryEntry>adaptIntEntry(const preg::PregEntry& entry,
                                                                             model::registry::RegistryEntryType type)
    {
        auto registryEntry = std::make_unique<model::registry::RegistryEntry<TData> >();
        registryEntry->key = entry.key.c_str();
        registryEntry->type = type;
        registryEntry->value = entry.value.c_str();
        if (entry.data) {
            registryEntry->data = static_cast<TData>(*entry.data);
            delete[] entry.data;
        }

        return registryEntry;
    }

public:
    static std::unique_ptr<model::registry::AbstractRegistryEntry> create(const preg::PregEntry& entry)
    {
        switch (entry.type)
        {
        case preg::REG_BINARY:
        {
            return adaptCharEntry(entry, model::registry::REG_BINARY);
        } break;

        case preg::REG_DWORD_LITTLE_ENDIAN:
        {
            return adaptIntEntry<uint32_t>(entry, model::registry::REG_DWORD);
        } break;

        case preg::REG_DWORD_BIG_ENDIAN:
        {
            return adaptIntEntry<uint32_t>(entry, model::registry::REG_DWORD_BIG_ENDIAN);
        } break;

        case preg::REG_EXPAND_SZ:
        {
            // TODO: Implement.
            return adaptCharEntry(entry, model::registry::REG_BINARY);
        } break;

        case preg::REG_LINK:
        {
            // TODO: Implement.
            return adaptCharEntry(entry, model::registry::REG_BINARY);
        } break;

        case preg::REG_MULTI_SZ:
        {
            // TODO: Implement.
            return adaptCharEntry(entry, model::registry::REG_BINARY);
        } break;

        case preg::REG_NONE:
        {
            return adaptCharEntry(entry, model::registry::REG_BINARY);
        } break;

        case preg::REG_QWORD:
        {
            return adaptIntEntry<uint64_t>(entry, model::registry::REG_QWORD);
        } break;

        case preg::REG_QWORD_LITTLE_ENDIAN:
        {
            return adaptIntEntry<uint64_t>(entry, model::registry::REG_QWORD);
        } break;

        case preg::REG_SZ:
        {
            // TODO: Implement.
            return adaptCharEntry(entry, model::registry::REG_BINARY);
        } break;

        default:
        {
            std::cout << "Unrecognized data type detected!" << std::endl;
            delete[] entry.data;
        } break;
        };

        return nullptr;
    }
};

PolFormat::PolFormat()
    : RegistryFileFormat("pol")
{

}

bool PolFormat::read(std::istream &input, io::RegistryFile* file)
{
    auto parser = std::make_unique<preg::PregParser>(input);

    auto registry = std::make_shared<model::registry::Registry>();

    try {
        preg::PregEntry entry;

        while (auto entryPointer = parser->getNextEntry()) {
            registry->registryEntries.push_back(RegistryEntryAdapter::create(*entryPointer));
        }
    } catch (preg::InvalidMagic& e) {
        setErrorString(e.what());
        return false;
    } catch (preg::InvalidVersion& e) {
        setErrorString(e.what());
        return false;
    }

    file->setRegistry(registry);

    return true;
}

bool PolFormat::write(std::ostream &output, io::RegistryFile* file)
{
    Q_UNUSED(output);
    Q_UNUSED(file);

    return false;
}

}
