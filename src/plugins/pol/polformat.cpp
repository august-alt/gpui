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
#include "pregwriter.h"

#include "../../../src/model/registry/registry.h"
#include "../../../src/model/registry/registryentry.h"
#include "../../../src/model/registry/registryentrytype.h"

using namespace model::registry;

namespace gpui {

class RegistryEntryAdapter
{
private:
    static std::unique_ptr<model::registry::AbstractRegistryEntry>adaptCharEntry(const preg::PregEntry& entry,
                                                                             model::registry::RegistryEntryType type)
    {
        auto registryEntry = std::make_unique<model::registry::RegistryEntry<QString> >();
        registryEntry->key = entry.key.c_str();
        registryEntry->type = type;
        registryEntry->value = entry.value.c_str();
        if (entry.data) {
            registryEntry->data = QString::fromLocal8Bit(entry.data, entry.size);
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

    static std::unique_ptr<model::registry::AbstractRegistryEntry>adaptMultiLineEntry(const preg::PregEntry& entry,
                                                                                model::registry::RegistryEntryType type)
    {
        auto registryEntry = std::make_unique<model::registry::RegistryEntry<QStringList> >();
        registryEntry->key = entry.key.c_str();
        registryEntry->type = type;
        registryEntry->value = entry.value.c_str();
        if (entry.data) {
            size_t size = entry.size - 2 > 0 ? entry.size - 2 : entry.size;
            QByteArray byteArray(entry.data, size);
            QList<QByteArray> arrayList = byteArray.split('\0');
            for (const auto& element : arrayList) {
                registryEntry->data.push_back(QString::fromLocal8Bit(element));
            }
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
            return adaptCharEntry(entry, model::registry::REG_EXPAND_SZ);
        } break;

        case preg::REG_LINK:
        {
            return adaptCharEntry(entry, model::registry::REG_BINARY);
        } break;

        case preg::REG_MULTI_SZ:
        {
            return adaptMultiLineEntry(entry, model::registry::REG_MULTI_SZ);
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

class PregEntryAdapter
{
public:
    static preg::PregEntry create(const std::unique_ptr<model::registry::AbstractRegistryEntry>& entry)
    {
        auto result = preg::PregEntry();
        result.key = entry->key.toStdString();
        result.value = entry->value.toStdString();
        result.type = entry->type;

        switch (entry->type) {
        case REG_BINARY:
        case REG_EXPAND_SZ:        
        case REG_SZ:
        {
            auto binaryEntry = static_cast<RegistryEntry<QString>* >(entry.get());
            char* stringData = new char[binaryEntry->data.size() + 1];
            memcpy(stringData, &binaryEntry->data.toStdString().c_str()[0], binaryEntry->data.size() + 1);
            stringData[binaryEntry->data.size()] = '\0';
            result.data = stringData;
            result.size = binaryEntry->data.size() + 1;
        } break;
        case REG_DWORD:
        case REG_DWORD_BIG_ENDIAN:
        {
            auto unit32Entry = static_cast<RegistryEntry<uint32_t>* >(entry.get());
            result.size = 4;
            result.data = reinterpret_cast<char*>(&unit32Entry->data);
        } break;
        case REG_QWORD:
        {
            auto unit64Entry = static_cast<RegistryEntry<uint64_t>* >(entry.get());
            result.size = 8;
            result.data = reinterpret_cast<char*>(&unit64Entry->data);
        }break;
        case REG_MULTI_SZ:
        {
            auto binaryEntry = static_cast<RegistryEntry<QStringList>* >(entry.get());
            QByteArray byteArray;
            for (const QString &str : binaryEntry->data)
            {
                byteArray.append(str);
                byteArray.append('\0');
            }
            if (byteArray.size() == 0)
            {
                byteArray.append("\0\0");
            }
            else
            {
                byteArray.append('\0');
            }
            char* stringData = new char[byteArray.size()];
            memcpy(stringData, byteArray, byteArray.size());
            result.data = stringData;
            result.size = byteArray.size();
        }break;
        default:
            break;
        }

        return result;
    }
};

PolFormat::PolFormat()
    : RegistryFileFormat("pol")
{

}

bool PolFormat::read(std::istream &input, io::RegistryFile* file)
{
    auto registry = std::make_shared<model::registry::Registry>();

    try {
        auto parser = std::make_unique<preg::PregParser>(input);

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
    auto writer = std::make_unique<preg::PregWriter>(&output);

    try {
        for (const auto& entry : file->getRegistry()->registryEntries)
        {
            auto pregEntry = PregEntryAdapter::create(entry);
            writer->addEntry(pregEntry);

            switch (pregEntry.type) {
            case REG_BINARY:
            case REG_EXPAND_SZ:
            case REG_MULTI_SZ:
            case REG_SZ:
            {
                delete [] pregEntry.data;
            }
            default:
                break;
            }
        }
    }  catch (std::exception& e) {
        setErrorString(e.what());
        return false;
    }

    return true;
}

}
