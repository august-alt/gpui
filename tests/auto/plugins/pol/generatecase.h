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
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "../../../../src/plugins/pol/encoding.h"
#include "../../../../src/plugins/pol/parser.h"
#include "./testcases.h"

std::string generateRandomKey(size_t length, std::mt19937 &gen)
{
    std::string key;
    key.resize(length);
    for (size_t i = 0; i < length; ++i) {
        // [0x20-\x5B] | [\x5D-\x7E]
        key[i] = (gen() % 0x5E) + 0x20;
        key[i] = key[i] >= 0x5c ? key[i] + 1 : key[i];
    }
    return key;
}

std::string generateRandomKeypath(std::mt19937 &gen)
{
    std::string keyPath;
    keyPath += generateRandomKey((gen() % 99) + 1, gen);

    while ((rand() % 5) >= 3) {
        keyPath += '\\';
        keyPath += generateRandomKey((gen() % 99) + 1, gen);
    }

    return keyPath;
}

std::string generateRandomValue(std::mt19937 &gen)
{
    std::string value;
    value.resize((gen() % 99) + 1);
    for (size_t i = 0; i < value.size(); ++i) {
        value[i] = (gen() % 0x5E) + 0x20;
    }
    return value;
}

pol::PolicyRegType generateRandomType(std::mt19937 &gen)
{
    switch (rand() % 7) {
    case 0:
        return pol::PolicyRegType::REG_BINARY;
    case 1:
        return pol::PolicyRegType::REG_DWORD_LITTLE_ENDIAN;
    case 2:
        return pol::PolicyRegType::REG_DWORD_BIG_ENDIAN;
    case 3:
        return pol::PolicyRegType::REG_QWORD_LITTLE_ENDIAN;
    case 4:
        return pol::PolicyRegType::REG_QWORD_BIG_ENDIAN;
    case 5:
        return pol::PolicyRegType::REG_SZ;
    case 6:
        return pol::PolicyRegType::REG_MULTI_SZ;
    default:
        break;
    }
    return pol::PolicyRegType::REG_BINARY;
}

pol::PolicyData generateRandomData(pol::PolicyRegType type, std::mt19937 &gen)
{
    iconv_t conv = iconv_open("UTF-8", "UTF-32LE");
    if (conv == (decltype(conv))-1) {
        return {};
    }

    switch (type) {
    case pol::PolicyRegType::REG_NONE:
        return {};
    case pol::PolicyRegType::REG_SZ: {
        std::basic_string<char32_t> data;
        data.resize(rand() % 100);
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] = (gen() % 0x5E) + 0x20;
        }
        auto result = pol::convert<char, char32_t>(data, conv);
        iconv_close(conv);
        return result;
    }

    case pol::PolicyRegType::REG_MULTI_SZ: {
        std::vector<std::string> data1;
        size_t count = rand() % 100;
        for (size_t i = 0; i < count; ++i) {
            std::basic_string<char32_t> data;
            data.resize((rand() % 100) + 1);
            for (size_t i = 0; i < data.size(); ++i) {
                data[i] = (gen() % 0x5E) + 0x20;
            }
            data1.push_back(pol::convert<char, char32_t>(data, conv));
        }
        iconv_close(conv);
        return data1;
    }

    case pol::PolicyRegType::REG_BINARY: {
        std::vector<uint8_t> data;
        size_t count = gen() % 100;
        for (size_t i = 0; i < count; ++i) {
            data.push_back((gen() % 255) + 1);
        }
        iconv_close(conv);
        return data;
    }

    case pol::PolicyRegType::REG_DWORD_LITTLE_ENDIAN:
        iconv_close(conv);
        return uint32_t(rand() % 10'000'000);
    case pol::PolicyRegType::REG_DWORD_BIG_ENDIAN:
        iconv_close(conv);
        return uint32_t(rand() % 10'000'000);
    case pol::PolicyRegType::REG_QWORD_LITTLE_ENDIAN:
        iconv_close(conv);
        return uint64_t(rand() % 10'000'000);
    case pol::PolicyRegType::REG_QWORD_BIG_ENDIAN:
        iconv_close(conv);
        return uint64_t(rand() % 10'000'000);
    default:
        iconv_close(conv);
        return {};
    }
}

void generateCase(size_t last, size_t seed = -1)
{
    std::mt19937 gen;
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 500);
    auto parser = pol::createPregParser();
    size_t current = 0;

    if (seed == -1) {
        std::random_device dev;
        seed = dev();
        gen.seed(seed);
    } else {
        gen.seed(seed);
    }

    qWarning() << "Begin test with generating cases. Seed:" << seed;

    while (current <= last) {
        std::stringstream file;

        // Generate case
        pol::PolicyFile data;
        data.body = std::make_optional<pol::PolicyBody>();
        size_t el = dist(gen);
        for (size_t i = 0; i < el; i++) {
            pol::PolicyInstruction instruction;
            instruction.type = generateRandomType(gen);
            instruction.data = generateRandomData(instruction.type, gen);
            data.body->instructions[generateRandomKeypath(gen)][generateRandomValue(gen)] =
                    std::move(instruction);
        }

        parser->write(file, data);
        file.seekg(0, std::ios::beg);

        auto test = parser->parse(file);
        if (data != test) {
            qCritical() << "one of generated files detect error in parser";
            assert(0);
        }

        qDebug() << "Generated" << current << "case: OK";

        ++current;
    }
}
