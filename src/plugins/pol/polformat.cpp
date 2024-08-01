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

#include "../../../src/plugins/administrative_templates/registry/registry.h"
#include "../../../src/plugins/administrative_templates/registry/registryentry.h"
#include "../../../src/plugins/administrative_templates/registry/registryentrytype.h"

#include "./parser.h"

#include <byteswap.h>
#include <tuple>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

using namespace model::registry;

namespace gpui {
class RegistryEntryAdapter
{
public:
    static void addInstruction(pol::PolicyTree &tree,
                               const std::unique_ptr<model::registry::AbstractRegistryEntry> &entry)
    {
        pol::PolicyInstruction instruction;

        auto key = entry->key.toStdString();
        auto value = entry->value.toStdString();

        if (tree.find(key) == tree.end()) {
            tree[key] = {};
        }
        if (tree[key].find(value) == tree[key].end()) {
            tree[key][value] = {};
        }

        switch (entry->type) {
        case model::registry::RegistryEntryType::REG_SZ: {
            auto tmp = static_cast<RegistryEntry<QString> *>(entry.get());
            instruction.type = pol::PolicyRegType::REG_SZ;
            instruction.data = tmp->data.toStdString();
            break;
        }
        case model::registry::RegistryEntryType::REG_EXPAND_SZ: {
            auto tmp = static_cast<RegistryEntry<QString> *>(entry.get());
            instruction.type = pol::PolicyRegType::REG_EXPAND_SZ;
            instruction.data = tmp->data.toStdString();
            break;
        }

        case model::registry::RegistryEntryType::REG_BINARY: {
            auto tmp = static_cast<RegistryEntry<QString> *>(entry.get());
            instruction.type = pol::PolicyRegType::REG_BINARY;

            auto tmp2 = tmp->data.toStdString();

            std::vector<uint8_t> data;
            data.resize(tmp->data.size());
            memcpy(data.data(), tmp2.data(), tmp2.size());
            instruction.data = std::move(data);
            break;
        }

        case model::registry::RegistryEntryType::REG_MULTI_SZ: {
            auto tmp = static_cast<RegistryEntry<QStringList> *>(entry.get());
            instruction.type = pol::PolicyRegType::REG_MULTI_SZ;
            std::vector<std::string> data;
            data.reserve(tmp->data.size());
            for (const auto &str : tmp->data) {
                data.push_back(str.toStdString());
            }
            instruction.data = std::move(data);
            break;
        }

        case model::registry::RegistryEntryType::REG_DWORD: {
            auto tmp = static_cast<RegistryEntry<quint32> *>(entry.get());
            instruction.type = pol::PolicyRegType::REG_DWORD_LITTLE_ENDIAN;
            instruction.data = static_cast<uint32_t>(tmp->data);
            break;
        }

        case model::registry::RegistryEntryType::REG_DWORD_BIG_ENDIAN: {
            auto tmp = static_cast<RegistryEntry<quint64> *>(entry.get());
            instruction.type = pol::PolicyRegType::REG_DWORD_BIG_ENDIAN;
            instruction.data = static_cast<uint32_t>(tmp->data);
            break;
        }
        case model::registry::RegistryEntryType::REG_QWORD: {
            auto tmp = static_cast<RegistryEntry<quint32> *>(entry.get());
            instruction.type = pol::PolicyRegType::REG_QWORD_LITTLE_ENDIAN;
            instruction.data = static_cast<uint64_t>(tmp->data);
            break;
        }
        default: {
            qWarning() << "Unrecognized data type `REG_NONE` detected! ";
            return;
        }
        }

        tree[key][value].emplace_back(instruction);
    }

    static std::unique_ptr<model::registry::AbstractRegistryEntry>
    create(const pol::PolicyInstruction &entry, const std::string &key, const std::string &value)
    {
        switch (entry.type) {
        case pol::PolicyRegType::REG_SZ: {
            auto registryEntry = std::make_unique<model::registry::RegistryEntry<QString>>();

            registryEntry->key = QString::fromStdString(key);
            registryEntry->value = QString::fromStdString(value);
            registryEntry->type = model::registry::RegistryEntryType::REG_SZ;
            registryEntry->data = QString::fromStdString(std::get<std::string>(entry.data));

            return registryEntry;
        }
        case pol::PolicyRegType::REG_EXPAND_SZ: {
            auto registryEntry = std::make_unique<model::registry::RegistryEntry<QString>>();

            registryEntry->key = QString::fromStdString(key);
            registryEntry->value = QString::fromStdString(value);
            registryEntry->type = model::registry::RegistryEntryType::REG_EXPAND_SZ;
            registryEntry->data = QString::fromStdString(std::get<std::string>(entry.data));

            return registryEntry;
        }
        case pol::PolicyRegType::REG_BINARY: {
            auto registryEntry = std::make_unique<model::registry::RegistryEntry<QString>>();

            registryEntry->key = QString::fromStdString(key);
            registryEntry->value = QString::fromStdString(value);
            registryEntry->type = model::registry::RegistryEntryType::REG_BINARY;
            auto &data = std::get<std::vector<uint8_t>>(entry.data);
            registryEntry->data = QString::fromStdString(
                    { reinterpret_cast<const char *>(data.data()), data.size() });

            return registryEntry;
        }
        case pol::PolicyRegType::REG_DWORD_LITTLE_ENDIAN: {
            auto registryEntry = std::make_unique<model::registry::RegistryEntry<uint32_t>>();

            registryEntry->key = QString::fromStdString(key);
            registryEntry->value = QString::fromStdString(value);
            registryEntry->type = model::registry::RegistryEntryType::REG_DWORD;
            registryEntry->data = std::get<uint32_t>(entry.data);

            return registryEntry;
        }
        case pol::PolicyRegType::REG_DWORD_BIG_ENDIAN: {
            auto registryEntry = std::make_unique<model::registry::RegistryEntry<uint32_t>>();

            registryEntry->key = QString::fromStdString(key);
            registryEntry->value = QString::fromStdString(value);
            registryEntry->type = model::registry::RegistryEntryType::REG_DWORD_BIG_ENDIAN;
            registryEntry->data = std::get<uint32_t>(entry.data);

            return registryEntry;
        }
        case pol::PolicyRegType::REG_LINK: {
            auto registryEntry = std::make_unique<model::registry::RegistryEntry<QString>>();

            registryEntry->key = QString::fromStdString(key);
            registryEntry->value = QString::fromStdString(value);
            registryEntry->type = model::registry::RegistryEntryType::REG_BINARY;

            auto &data = registryEntry->data =
                    QString::fromStdString(std::get<std::string>(entry.data));

            return registryEntry;
        }
        case pol::PolicyRegType::REG_MULTI_SZ: {
            auto registryEntry = std::make_unique<model::registry::RegistryEntry<QStringList>>();

            registryEntry->key = QString::fromStdString(key);
            registryEntry->value = QString::fromStdString(value);
            registryEntry->type = model::registry::RegistryEntryType::REG_MULTI_SZ;

            auto &data = std::get<std::vector<std::string>>(entry.data);
            registryEntry->data.reserve(data.size());
            for (const auto &datum : data) {
                registryEntry->data.append(QString::fromStdString(datum));
            }

            return registryEntry;
        }
        case pol::PolicyRegType::REG_QWORD_BIG_ENDIAN:
        case pol::PolicyRegType::REG_QWORD_LITTLE_ENDIAN: {
            auto registryEntry = std::make_unique<model::registry::RegistryEntry<uint64_t>>();

            registryEntry->key = QString::fromStdString(key);
            registryEntry->value = QString::fromStdString(value);
            registryEntry->type = model::registry::RegistryEntryType::REG_QWORD;
            registryEntry->data = std::get<uint64_t>(entry.data);

            return registryEntry;
        }
        case pol::PolicyRegType::REG_RESOURCE_LIST:
        case pol::PolicyRegType::REG_FULL_RESOURCE_DESCRIPTOR:
        case pol::PolicyRegType::REG_RESOURCE_REQUIREMENTS_LIST:
        default:
        case pol::PolicyRegType::REG_NONE:
            qWarning() << "Unrecognized data type detected! " << static_cast<int>(entry.type);
            return nullptr;
        }
    }
};

PolFormat::PolFormat() : RegistryFileFormat("pol") { }

bool PolFormat::read(std::istream &input, io::RegistryFile *file)
{
    auto registry = std::make_shared<model::registry::Registry>();

    auto parser = pol::createPregParser();
    pol::PolicyFile result;
    try {
        result = parser->parse(input);
    } catch (const std::exception &e) {
        qWarning() << e.what();
        return false;
    }

    for (const auto &[key, record] : result.body->instructions) {
        for (const auto &[value, array] : record) {
            for(const auto &entry : array) {
                auto registryEntry = RegistryEntryAdapter::create(entry, key, value);
                if (registryEntry.get()) {
                    registry->registryEntries.push_back(std::move(registryEntry));
                }
            }
        }
    }

    file->setRegistry(registry);

    return true;
}

bool PolFormat::write(std::ostream &output, io::RegistryFile *file)
{
    auto writer = pol::createPregParser();
    auto result = pol::PolicyFile();
    result.body = std::make_optional<pol::PolicyBody>();

    for (const auto &entry : file->getRegistry()->registryEntries) {
        RegistryEntryAdapter::addInstruction(result.body->instructions, entry);
    }

    try {
        writer->write(output, result);
    } catch (const std::exception &e) {
        qWarning() << e.what();
        return false;
    }

    return true;
}

} // namespace gpui

#pragma GCC diagnostic pop
