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

#include "../../core/logger/log.h"

#include "pregdata.h"
#include "pregparser.h"
#include "pregwriter.h"

#include "../../../src/plugins/administrative_templates/registry/registry.h"
#include "../../../src/plugins/administrative_templates/registry/registryentry.h"
#include "../../../src/plugins/administrative_templates/registry/registryentrytype.h"

#include <byteswap.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

using namespace model::registry;

namespace gpui
{
class RegistryEntryAdapter
{
private:
    static std::unique_ptr<model::registry::AbstractRegistryEntry> adaptCharEntry(
        const preg::PregEntry &entry, model::registry::RegistryEntryType type)
    {
        auto registryEntry   = std::make_unique<model::registry::RegistryEntry<QString>>();
        registryEntry->key   = entry.key.c_str();
        registryEntry->type  = type;
        registryEntry->value = entry.value.c_str();
        if (entry.data)
        {
            void *pt            = entry.data;
            size_t requiredSize = entry.size;
            registryEntry->data = QString::fromUtf16(
                reinterpret_cast<char16_t *>(std::align(alignof(char16_t), sizeof(char), pt, requiredSize)));
            delete[] entry.data;
        }

        return registryEntry;
    }

    static std::unique_ptr<model::registry::AbstractRegistryEntry> adaptUInt32Entry(
        const preg::PregEntry &entry, model::registry::RegistryEntryType type, bool bigEndian = false)
    {
        auto registryEntry   = std::make_unique<model::registry::RegistryEntry<uint32_t>>();
        registryEntry->key   = entry.key.c_str();
        registryEntry->type  = type;
        registryEntry->value = entry.value.c_str();
        if (entry.data)
        {
            uint32_t data = (uint32_t)((uint8_t) entry.data[0]) | (uint32_t)((uint8_t) entry.data[1]) << 8
                            | (uint32_t)((uint8_t) entry.data[2]) << 16 | (uint32_t)((uint8_t) entry.data[3]) << 24;

            if (bigEndian)
            {
                data = bswap_32(data);
            }
            registryEntry->data = data;
            delete[] entry.data;
        }

        return registryEntry;
    }

    static std::unique_ptr<model::registry::AbstractRegistryEntry> adaptUInt64Entry(
        const preg::PregEntry &entry, model::registry::RegistryEntryType type, bool bigEndian = false)
    {
        auto registryEntry   = std::make_unique<model::registry::RegistryEntry<uint64_t>>();
        registryEntry->key   = entry.key.c_str();
        registryEntry->type  = type;
        registryEntry->value = entry.value.c_str();
        if (entry.data)
        {
            uint64_t data = (uint64_t)((uint8_t) entry.data[0]) | (uint64_t)((uint8_t) entry.data[1]) << 8
                            | (uint64_t)((uint8_t) entry.data[2]) << 16 | (uint64_t)((uint8_t) entry.data[3]) << 24
                            | (uint64_t)((uint8_t) entry.data[4]) << 32 | (uint64_t)((uint8_t) entry.data[5]) << 40
                            | (uint64_t)((uint8_t) entry.data[6]) << 48 | (uint64_t)((uint8_t) entry.data[7]) << 56;
            if (bigEndian)
            {
                data = bswap_64(data);
            }
            registryEntry->data = data;
            delete[] entry.data;
        }

        return registryEntry;
    }

    static std::unique_ptr<model::registry::AbstractRegistryEntry> adaptMultiLineEntry(
        const preg::PregEntry &entry, model::registry::RegistryEntryType type)
    {
        auto registryEntry   = std::make_unique<model::registry::RegistryEntry<QStringList>>();
        registryEntry->key   = entry.key.c_str();
        registryEntry->type  = type;
        registryEntry->value = entry.value.c_str();
        if (entry.data)
        {
            size_t size          = entry.size - 2 > 0 ? entry.size - 2 : entry.size;
            void *pt             = entry.data;
            size_t requiredSize  = entry.size;
            const char16_t *data = reinterpret_cast<const char16_t *>(
                std::align(alignof(char16_t), sizeof(char), pt, requiredSize));
            std::vector<std::vector<char16_t>> list;
            std::vector<char16_t> current;
            GPUI_WARNING_STREAM << "Data: "
                       << QString::fromUtf16(reinterpret_cast<char16_t *>(
                                                 std::align(alignof(char16_t), sizeof(char), pt, requiredSize)),
                                             size / 2)
                       << " size: " << size;
            for (size_t i = 0; i < size / 2; ++i)
            {
                current.push_back(data[i]);
                if (data[i] == 0)
                {
                    GPUI_WARNING_STREAM << current << " split at: " << i;
                    list.emplace_back(current);
                    current.clear();
                }
            }
            for (const auto &element : list)
            {
                registryEntry->data.push_back(QString::fromUtf16(element.data()));
            }
            delete[] entry.data;
        }

        return registryEntry;
    }

public:
    static std::unique_ptr<model::registry::AbstractRegistryEntry> create(const preg::PregEntry &entry)
    {
        switch (entry.type)
        {
        case preg::REG_BINARY: {
            return adaptCharEntry(entry, model::registry::REG_BINARY);
        }
        break;

        case preg::REG_DWORD_LITTLE_ENDIAN: {
            return adaptUInt32Entry(entry, model::registry::REG_DWORD);
        }
        break;

        case preg::REG_DWORD_BIG_ENDIAN: {
            return adaptUInt32Entry(entry, model::registry::REG_DWORD_BIG_ENDIAN, true);
        }
        break;

        case preg::REG_EXPAND_SZ: {
            return adaptCharEntry(entry, model::registry::REG_EXPAND_SZ);
        }
        break;

        case preg::REG_LINK: {
            return adaptCharEntry(entry, model::registry::REG_BINARY);
        }
        break;

        case preg::REG_MULTI_SZ: {
            return adaptMultiLineEntry(entry, model::registry::REG_MULTI_SZ);
        }
        break;

        case preg::REG_NONE: {
            return adaptCharEntry(entry, model::registry::REG_BINARY);
        }
        break;

        case preg::REG_QWORD: {
            return adaptUInt64Entry(entry, model::registry::REG_QWORD, true);
        }
        break;

        case preg::REG_QWORD_LITTLE_ENDIAN: {
            return adaptUInt64Entry(entry, model::registry::REG_QWORD);
        }
        break;

        case preg::REG_SZ: {
            return adaptCharEntry(entry, model::registry::REG_SZ);
        }
        break;

        default: {
            std::cout << "Unrecognized data type detected!" << std::endl;
            delete[] entry.data;
        }
        break;
        };

        return nullptr;
    }
};

class PregEntryAdapter
{
public:
    static preg::PregEntry create(const std::unique_ptr<model::registry::AbstractRegistryEntry> &entry)
    {
        auto result  = preg::PregEntry();
        result.key   = entry->key.toStdString();
        result.value = entry->value.toStdString();
        result.type  = entry->type;

        switch (entry->type)
        {
        case REG_BINARY:
        case REG_EXPAND_SZ:
        case REG_SZ: {
            auto binaryEntry      = static_cast<RegistryEntry<QString> *>(entry.get());
            auto sixteenBitString = binaryEntry->data.toStdU16String();
            size_t bufferSize     = sixteenBitString.size() * sizeof(char16_t);
            char *stringData      = new char[bufferSize + 2];
            memcpy(stringData, sixteenBitString.c_str(), bufferSize);
            stringData[bufferSize]     = '\0';
            stringData[bufferSize + 1] = '\0';
            result.data                = stringData;
            result.size                = bufferSize + 2;
        }
        break;
        case REG_DWORD:
        case REG_DWORD_BIG_ENDIAN: {
            auto uint32Entry  = static_cast<RegistryEntry<uint32_t> *>(entry.get());
            result.size       = 4;
            size_t bufferSize = sizeof(uint32_t);
            char *stringData  = new char[bufferSize];
            stringData[0]     = (uint8_t)((uint8_t *) (&uint32Entry->data))[0];
            stringData[1]     = (uint8_t)((uint8_t *) (&uint32Entry->data))[1];
            stringData[2]     = (uint8_t)((uint8_t *) (&uint32Entry->data))[2];
            stringData[3]     = (uint8_t)((uint8_t *) (&uint32Entry->data))[3];

            result.data = stringData;
        }
        break;
        case REG_QWORD: {
            auto uint64Entry  = static_cast<RegistryEntry<uint64_t> *>(entry.get());
            result.size       = 8;
            size_t bufferSize = sizeof(uint64_t);
            char *stringData  = new char[bufferSize];
            stringData[0]     = (uint8_t)((uint8_t *) (&uint64Entry->data))[0];
            stringData[1]     = (uint8_t)((uint8_t *) (&uint64Entry->data))[1];
            stringData[2]     = (uint8_t)((uint8_t *) (&uint64Entry->data))[2];
            stringData[3]     = (uint8_t)((uint8_t *) (&uint64Entry->data))[3];
            stringData[4]     = (uint8_t)((uint8_t *) (&uint64Entry->data))[4];
            stringData[5]     = (uint8_t)((uint8_t *) (&uint64Entry->data))[5];
            stringData[6]     = (uint8_t)((uint8_t *) (&uint64Entry->data))[6];
            stringData[7]     = (uint8_t)((uint8_t *) (&uint64Entry->data))[7];
            result.data       = stringData;
        }
        break;
        case REG_MULTI_SZ: {
            auto binaryEntry = static_cast<RegistryEntry<QStringList> *>(entry.get());
            QByteArray byteArray;
            for (const QString &str : binaryEntry->data)
            {
                auto sixteenBitString = str.toStdU16String();
                size_t size           = sixteenBitString.size() * sizeof(char16_t);
                byteArray.append(reinterpret_cast<const char *>(sixteenBitString.c_str()), size);
                byteArray.append(2, '\0');
            }
            if (byteArray.size() == 0)
            {
                byteArray.append(4, '\0');
            }
            else
            {
                byteArray.append(2, '\0');
            }
            char *stringData = new char[byteArray.size()];
            memcpy(stringData, byteArray, byteArray.size());
            result.data = stringData;
            result.size = byteArray.size();
        }
        break;
        default:
            break;
        }

        return result;
    }
};

PolFormat::PolFormat()
    : RegistryFileFormat("pol")
{}

bool PolFormat::read(std::istream &input, io::RegistryFile *file)
{
    auto registry = std::make_shared<model::registry::Registry>();

    try
    {
        auto parser = std::make_unique<preg::PregParser>(input);

        preg::PregEntry entry;

        while (auto entryPointer = parser->getNextEntry())
        {
            registry->registryEntries.push_back(RegistryEntryAdapter::create(*entryPointer));
        }
    }
    catch (preg::InvalidMagic &e)
    {
        setErrorString(e.what());
        return false;
    }
    catch (preg::InvalidVersion &e)
    {
        setErrorString(e.what());
        return false;
    }

    file->setRegistry(registry);

    return true;
}

bool PolFormat::write(std::ostream &output, io::RegistryFile *file)
{
    auto writer = std::make_unique<preg::PregWriter>(&output);

    try
    {
        for (const auto &entry : file->getRegistry()->registryEntries)
        {
            auto pregEntry = PregEntryAdapter::create(entry);
            writer->addEntry(pregEntry);

            switch (pregEntry.type)
            {
            case REG_DWORD:
            case REG_DWORD_BIG_ENDIAN:
            case REG_QWORD:
            case REG_BINARY:
            case REG_EXPAND_SZ:
            case REG_MULTI_SZ:
            case REG_SZ: {
                delete[] pregEntry.data;
            }
            default:
                break;
            }
        }
    }
    catch (std::exception &e)
    {
        setErrorString(e.what());
        return false;
    }

    return true;
}

} // namespace gpui

#pragma GCC diagnostic pop
