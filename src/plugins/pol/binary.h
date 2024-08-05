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

#ifndef PREGPARSER_BINARY
#define PREGPARSER_BINARY

#include <cinttypes>
#include <iostream>
#include <optional>
#include <vector>

#include "encoding.h"

namespace pol {

/*!
 * \brief Get string from istream (binary)
 * if conv == nullptr, then conv will be initialized inside by `iconv_open("UTF-8", "UTF-16LE")`
 * \return on any error return empty optional
 * \warning string in buffer must be ended with '\0'
 * \warning `conv` must be initialized by `iconv_open("UTF-8", "UTF-16LE")`
 * \warning if `conv` is (size_t)-1, then function will throw std::runtime_error
 */
std::string bufferToString(std::istream &buffer, size_t size, iconv_t conv = nullptr);
/*!
 * \brief Put string from istream (binary)
 * if conv == nullptr, then conv will be initialized inside by `iconv_open("UTF-8", "UTF-16LE")`
 * \return Size of writed string. On any error return (size_t)-1
 * \warning string in buffer will be ended with '\0'
 * \warning `conv` must be initialized by `iconv_open("UTF-16LE", "UTF-8")`
 * \warning if `conv` is (size_t)-1, then function will throw std::runtime_error
 */
size_t stringToBuffer(std::ostream &buffer, const std::string &data, iconv_t conv = nullptr);

/*!
 * \brief Get strings from istream (binary)
 * if conv == nullptr, then conv will be initialized inside by `iconv_open("UTF-8", "UTF-16LE")`
 * \return on any error return empty optional
 * \warning every strings in buffer must be ended with '\0' (last included)
 * \warning `conv` must be initialized by `iconv_open("UTF-8", "UTF-16LE")`
 * \warning if `conv` is (size_t)-1, then function will throw std::runtime_error
 */
std::vector<std::string> bufferToStrings(std::istream &buffer, size_t size, iconv_t conv = nullptr);
/*!
 * \brief Put string from istream (binary)
 * if conv == nullptr, then conv will be initialized inside by `iconv_open("UTF-8", "UTF-16LE")`
 * \return Size of writed strings. On any error return (size_t)-1
 * \warning every string in buffer will be ended with '\0' (last included)
 * \warning `conv` must be initialized by `iconv_open("UTF-16LE", "UTF-8")`
 * \warning if `conv` is (size_t)-1, then function will throw std::runtime_error
 */
size_t stringsToBuffer(std::ostream &buffer, const std::vector<std::string> &data,
                       iconv_t conv = nullptr);
/*!
 * \brief Get vector of raw data from istream (binary)
 */
std::vector<uint8_t> bufferToVector(std::istream &buffer, size_t size);

/*!
 * \brief Put vector of raw data to istream (binary)
 */
void vectorToBuffer(std::ostream &buffer, const std::vector<uint8_t> &data);

/*!
 * \brief Get integral number from istream (binary)
 */
template <typename T, bool LE = true,
          typename = std::enable_if_t<std::is_integral_v<T>
                                      && sizeof(T) <= sizeof(unsigned long long)>>
T bufferToIntegral(std::istream &buffer)
{
    T num = 0;

    buffer.read(reinterpret_cast<char *>(&num), sizeof(T));
    if (buffer.fail()) {
        if (buffer.eof()) {
            throw std::runtime_error(
                    "LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                    + ", Failed to read integral number from buffer, EOF was encountered.");
        }
        throw std::runtime_error(
                "LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                + ", Failed to read integral number from buffer, error was encountered.");
    }
    if constexpr (LE) {
        return leToNative<T>(num);
    } else {
        return beToNative<T>(num);
    }

    return num;
}

/*!
 * \brief Put integral number to ostream (binary)
 */
template <typename T, bool LE = true,
          typename = std::enable_if_t<std::is_integral_v<T>
                                      && sizeof(T) <= sizeof(unsigned long long)>>
void integralToBuffer(std::ostream &buffer, T num)
{
    if constexpr (LE) {
        num = nativeToLe<T>(num);
    } else {
        num = nativeToBe<T>(num);
    }

    buffer.write(reinterpret_cast<char *>(&num), sizeof(T));
    if (buffer.fail()) {
        throw std::runtime_error(
                "LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                + ", Failed to write integral number to buffer, error was encountered.");
    }
}

} // namespace pol

#endif // PREGPARSER_BINARY
