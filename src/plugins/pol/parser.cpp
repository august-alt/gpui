/*
 * libparsepol - POL Registry file parser
 *
 * Copyright (C) 2024 BaseALT Ltd.
 * Copyright (C) 2020 Korney Yakovlevich
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../../../src/core/common.h"
#include <sstream>
#include <vector>

#include "./binary.h"
#include "./common.h"
#include "./parser.h"

namespace pol {

/*!
 * \brief Valid POL Registery file header
 */
static const char valid_header[8] = { 0x50, 0x52, 0x65, 0x67, 0x01, 0x00, 0x00, 0x00 };

/*!
 * \brief Match regex `[\x20-\x7E]`
 */
static inline bool isValueCharacter(uint8_t sym)
{
    return sym >= 0x20 && sym <= 0x7E;
}

PRegParser::PRegParser()
{
    this->m_iconv_read_id = ::iconv_open("UTF-8", "UTF-16LE");
    this->m_iconv_write_id = ::iconv_open("UTF-16LE", "UTF-8");
}

PolicyFile PRegParser::parse(std::istream &stream)
{
    PolicyBody body;

    parseHeader(stream);

    stream.peek();
    while (!stream.eof()) {
        insertInstruction(stream, body.instructions);
        stream.peek();
    }

    return { body };
}

bool PRegParser::write(std::ostream &stream, const PolicyFile &file)
{
    if (!file.body.has_value()) {
        return true;
    }

    writeHeader(stream);
    for (const auto &[key, records] : file.body->instructions) {
        for (const auto &[value, array] : records) {
            for (const auto &instruction : array) {
                writeInstruction(stream, instruction, key, value);
            }
        }
    }

    return true;
}

PRegParser::~PRegParser()
{
    ::iconv_close(this->m_iconv_read_id);
    ::iconv_close(this->m_iconv_write_id);
}

void PRegParser::parseHeader(std::istream &stream)
{
    char header[8];
    stream.read(header, 8);
    check_stream(stream);

    const uint32_t signature = *reinterpret_cast<uint32_t *>(&header[0]);
    const uint32_t version = *reinterpret_cast<uint32_t *>(&header[4]);
    const uint32_t normal_signature = *reinterpret_cast<const uint32_t *>(&valid_header[0]);
    const uint32_t normal_version = *reinterpret_cast<const uint32_t *>(&valid_header[4]);

    if (signature != normal_signature && version != normal_version) {
        throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                                 + ", Encountered with invalid header.");
    }
}

uint32_t PRegParser::getSize(std::istream &stream)
{
    return bufferToIntegral<uint32_t, true>(stream);
}

PolicyRegType PRegParser::getType(std::istream &stream)
{
    uint32_t num = bufferToIntegral<uint32_t, true>(stream);

    switch (static_cast<PolicyRegType>(num)) {
    case PolicyRegType::REG_SZ:
    case PolicyRegType::REG_EXPAND_SZ:
    case PolicyRegType::REG_BINARY:
    case PolicyRegType::REG_DWORD_LITTLE_ENDIAN:
    case PolicyRegType::REG_DWORD_BIG_ENDIAN:
    case PolicyRegType::REG_LINK:
    case PolicyRegType::REG_MULTI_SZ:
    case PolicyRegType::REG_RESOURCE_LIST:
    case PolicyRegType::REG_FULL_RESOURCE_DESCRIPTOR:
    case PolicyRegType::REG_RESOURCE_REQUIREMENTS_LIST:
    case PolicyRegType::REG_QWORD_LITTLE_ENDIAN:
    case PolicyRegType::REG_QWORD_BIG_ENDIAN:
        break;
    default:
        return {};
    }

    return static_cast<PolicyRegType>(num);
}

std::string PRegParser::getKey(std::istream &stream)
{
    std::string key;
    char16_t data;

    stream.read(reinterpret_cast<char *>(&data), 2);
    check_stream(stream);
    data = leToNative(data);

    while (data >= 0x20 && data <= 0x7E && data != 0x5C) {
        key.push_back(static_cast<char>(data));

        stream.read(reinterpret_cast<char *>(&data), 2);
        check_stream(stream);
        data = leToNative(data);
    }

    // Key from Keypath must contain 1 or more symbols.
    if (key.empty() || (data != 0 && data != 0x5C)) {
        throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                                 + ", Unexpected symbol with code " + std::to_string(data) + ".");
    }

    // Remove last symbol
    stream.seekg(-2, std::ios::cur);

    return { key };
}

std::string PRegParser::getKeypath(std::istream &stream)
{
    std::string keyPath;
    char16_t sym = 0;

    while (true) {
        auto key = getKey(stream);

        keyPath.append(key);

        stream.read(reinterpret_cast<char *>(&sym), 2);
        check_stream(stream);

        // End of Keypath
        if (sym == 0) {
            break;
        }

        // This if never be executed, but for safety i use it
        if (sym != 0x5C) {
            throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                                     + ", Unexpected symbol with code " + std::to_string(sym)
                                     + ".");
        }

        keyPath.push_back('\\');
    }

    return { keyPath };
}

std::string PRegParser::getValue(std::istream &stream)
{
    std::string result;
    char16_t data;

    stream.read(reinterpret_cast<char *>(&data), 2);
    check_stream(stream);
    data = leToNative(data);

    // Key in specs [\x20-\x5B\x5D-\x7E](exclude '\'), when keypath include '\' like delimeter
    while (data >= 0x20 && data <= 0x7E) {
        // Key from Keypath must contain 1 or more symbols.

        // Check maximum value length
        if (result.length() == 259) {
            throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                                     + ", Unexpected symbol with code " + std::to_string(data)
                                     + ".");
        }

        result.push_back(data);

        stream.read(reinterpret_cast<char *>(&data), 2);
        check_stream(stream);
        data = leToNative(data);
    }

    if (data != 0 || result.empty()) {
        return {};
    }

    return { result };
}

PolicyData PRegParser::getData(std::istream &stream, PolicyRegType type, uint32_t size)
{
    switch (type) {
    case PolicyRegType::REG_NONE:
        throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                                 + ", Unexpected type REG_NONE.");
    case PolicyRegType::REG_SZ:
    case PolicyRegType::REG_EXPAND_SZ:
    case PolicyRegType::REG_LINK:
        return { bufferToString(stream, size, this->m_iconv_read_id) };

    case PolicyRegType::REG_BINARY:
        return { bufferToVector(stream, size) };

    case PolicyRegType::REG_DWORD_LITTLE_ENDIAN:
        return { bufferToIntegral<uint32_t, true>(stream) };
    case PolicyRegType::REG_DWORD_BIG_ENDIAN:
        return { bufferToIntegral<uint32_t, false>(stream) };

    case PolicyRegType::REG_MULTI_SZ:
    case PolicyRegType::REG_RESOURCE_LIST:
    case PolicyRegType::REG_FULL_RESOURCE_DESCRIPTOR: // ????
    case PolicyRegType::REG_RESOURCE_REQUIREMENTS_LIST:
        return { bufferToStrings(stream, size, this->m_iconv_read_id) };

    case PolicyRegType::REG_QWORD_LITTLE_ENDIAN:
        return { bufferToIntegral<uint64_t, true>(stream) };
    case PolicyRegType::REG_QWORD_BIG_ENDIAN:
        return { bufferToIntegral<uint64_t, false>(stream) };
        break;
    }
    return {};
}

void PRegParser::insertInstruction(std::istream &stream, PolicyTree &tree)
{
    PolicyInstruction instruction;
    uint32_t dataSize;

    check_sym(stream, '[');

    std::string keyPath = getKeypath(stream);

    check_sym(stream, ';');

    std::string value = getValue(stream);

    try {
        check_sym(stream, ';');

        instruction.type = getType(stream);

        check_sym(stream, ';');

        dataSize = getSize(stream);

        check_sym(stream, ';');

        instruction.data = getData(stream, instruction.type, dataSize);

        check_sym(stream, ']');

        if (tree.find(keyPath) == tree.end()) {
            tree[keyPath] = {};
        }
        if (tree[keyPath].find(value) == tree[keyPath].end()) {
            tree[keyPath][value] = {};
        }
        tree[keyPath][value].emplace_back(std::move(instruction));

    } catch (const std::exception &e) {
        throw std::runtime_error(std::string(e.what()) + "\nLINE: " + std::to_string(__LINE__)
                                 + ", FILE: " + __FILE__
                                 + ", Error was encountered wile parsing instruction with key: "
                                 + keyPath + ", value: " + value);
    }
}

std::stringstream PRegParser::getDataStream(const PolicyData &data, PolicyRegType type)
{
    std::stringstream stream;

    switch (type) {
    case PolicyRegType::REG_SZ:
    case PolicyRegType::REG_EXPAND_SZ:
    case PolicyRegType::REG_LINK:
        stringToBuffer(stream, std::get<std::string>(data), this->m_iconv_write_id);
        break;

    case PolicyRegType::REG_BINARY:
        vectorToBuffer(stream, std::get<std::vector<uint8_t>>(data));
        break;

    case PolicyRegType::REG_DWORD_LITTLE_ENDIAN:
        integralToBuffer<uint32_t, true>(stream, std::get<uint32_t>(data));
        break;
    case PolicyRegType::REG_DWORD_BIG_ENDIAN:
        integralToBuffer<uint32_t, false>(stream, std::get<uint32_t>(data));
        break;

    case PolicyRegType::REG_MULTI_SZ:
    case PolicyRegType::REG_RESOURCE_LIST:
    case PolicyRegType::REG_FULL_RESOURCE_DESCRIPTOR: // ????
    case PolicyRegType::REG_RESOURCE_REQUIREMENTS_LIST:
        stringsToBuffer(stream, std::get<std::vector<std::string>>(data), this->m_iconv_write_id);
        break;

    case PolicyRegType::REG_QWORD_LITTLE_ENDIAN:
        integralToBuffer<uint64_t, true>(stream, std::get<uint64_t>(data));
        break;
    case PolicyRegType::REG_QWORD_BIG_ENDIAN:
        integralToBuffer<uint64_t, false>(stream, std::get<uint64_t>(data));
        break;

    case PolicyRegType::REG_NONE:
        throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                                 + ", Unexpected type REG_NONE.");
    default:
        throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                                 + ", Unexpected type UNKNOWN("
                                 + std::to_string(static_cast<size_t>(type)) + ".");
    }

    return stream;
}

void PRegParser::writeHeader(std::ostream &stream)
{
    stream.write(valid_header, sizeof(valid_header));
}

void PRegParser::writeInstruction(std::ostream &stream, const PolicyInstruction &instruction,
                                  std::string key, std::string value)
{

    try {
        write_sym(stream, '[');

        stringToBuffer(stream, key);

        write_sym(stream, ';');

        stringToBuffer(stream, value);

        write_sym(stream, ';');

        integralToBuffer<uint32_t, true>(stream, static_cast<uint32_t>(instruction.type));

        write_sym(stream, ';');

        auto dataStream = getDataStream(instruction.data, instruction.type);

        integralToBuffer<uint32_t, true>(stream, static_cast<uint32_t>(dataStream.tellp()));

        write_sym(stream, ';');

        stream << dataStream.str();
        check_stream(stream);

        write_sym(stream, ']');
    } catch (const std::exception &e) {
        throw std::runtime_error(std::string(e.what()) + "\nLINE: " + std::to_string(__LINE__)
                                 + ", FILE: " + __FILE__
                                 + ", Error was encountered while writing instruction with key: "
                                 + key + ", value: " + value);
    }
}

GPUI_SYMBOL_EXPORT std::unique_ptr<PRegParser> createPregParser()
{
    return std::make_unique<PRegParser>();
}

} // namespace pol