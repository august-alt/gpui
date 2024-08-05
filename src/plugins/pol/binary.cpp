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
#include <iostream>

#include "binary.h"
#include "common.h"

namespace pol {

std::string bufferToString(std::istream &buffer, size_t size, iconv_t conv)
{
    bool custom_conv = false;
    if (conv == nullptr) {
        conv = iconv_open("UTF-8", "UTF-16LE");
        custom_conv = true;
    }

    if (conv == reinterpret_cast<iconv_t>(-1)) {
        throw std::runtime_error(
                "LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                + ", Encountered with the inability to create a iconv descriptor.");
    }

    std::basic_string<char16_t> source((size / 2) - 1, '\0');

    // std::the string contains '\0' at the end (C style), which means that the actual buffer size
    // is `size`. We read `size` bytes to check that the buffer ends with '\0'.
    // '\0' is included in `size`, so we read `size` bytes.
    buffer.read(reinterpret_cast<char *>(source.data()), size);
    check_stream(buffer);

    // Check that the buffer ends with the two '\0'.
    if (source.data()[(size / 2) - 1] != 0) {
        throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                                 + ", Encountered with invalid UTF-16LE buffer.");
    }

    auto result = convert<char, char16_t>(source, conv);
    if (custom_conv) {
        iconv_close(conv);
    }
    return result;
}

size_t stringToBuffer(std::ostream &buffer, const std::string &source, iconv_t conv)
{
    bool custom_conv = false;
    if (conv == nullptr) {
        conv = iconv_open("UTF-16LE", "UTF-8");
        custom_conv = true;
    }

    if (conv == reinterpret_cast<iconv_t>(-1)) {
        throw std::runtime_error(
                "LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                + ", Encountered with the inability to create a iconv descriptor.");
    }

    std::basic_string<char16_t> converted = convert<char16_t, char>(source, conv);

    buffer.write(reinterpret_cast<char *>(converted.data()),
                 (converted.size() + 1) * sizeof(char16_t));
    check_stream(buffer);

    if (custom_conv) {
        iconv_close(conv);
    }
    return (converted.size() + 1) * sizeof(char16_t);
}

std::vector<std::string> bufferToStrings(std::istream &buffer, size_t size, iconv_t conv)
{
    bool custom_conv = false;

    if (size == 0) {
        return {};
    }
    if (conv == nullptr) {
        conv = iconv_open("UTF-8", "UTF-16LE");
        custom_conv = true;
    }

    if (conv == reinterpret_cast<iconv_t>(-1)) {
        throw std::runtime_error(
                "LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                + ", Encountered with the inability to create a iconv descriptor.");
    }

    std::vector<std::string> result;
    std::basic_string<char16_t> tmp;
    size_t current = 0;
    size_t found = 0;

    // std::string contains '\0' at the end (C style), which means that the actual buffer size
    // is `size`. We read `size` bytes to check that the buffer ends with '\0'.
    // '\0' is included in `size`, so we read `size` bytes.
    tmp.resize((size / 2) - 1);
    buffer.read(reinterpret_cast<char *>(tmp.data()), size);
    check_stream(buffer);

    while (found <= tmp.size()) {
        found = tmp.find(char16_t(0), current);

        if (found == std::string::npos) {
            found = tmp.size();

            if (tmp.data()[found] != 0) {
                return {};
            }
        }

        result.push_back(
                convert<char, char16_t>(tmp.cbegin() + current, tmp.cbegin() + found, conv));

        current = found + 1;
        found = current;
    }

    if (custom_conv) {
        iconv_close(conv);
    }
    return result;
}

size_t stringsToBuffer(std::ostream &buffer, const std::vector<std::string> &data, iconv_t conv)
{
    size_t size = 0;

    for (const auto &str : data) {
        auto tmp = stringToBuffer(buffer, str, conv);
        size += tmp;
    }

    return size;
}

std::vector<uint8_t> bufferToVector(std::istream &buffer, size_t size)
{
    std::vector<uint8_t> result;
    result.resize(size);

    buffer.read(reinterpret_cast<char *>(result.data()), size);
    check_stream(buffer);

    return result;
}

void vectorToBuffer(std::ostream &buffer, const std::vector<uint8_t> &data)
{
    buffer.write(reinterpret_cast<const char *>(data.data()), data.size());
    check_stream(buffer);
}

} // namespace pol
