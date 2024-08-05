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

#ifndef PREGPARSER_ENCODING
#define PREGPARSER_ENCODING

#include <array>
#include <cerrno>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string.h>
#include <type_traits>

#include "iconv.h"

namespace pol {

// C++ before C++20 is not support endianess. Becouse of that, we need to
// provide our own implementation

enum class Endian {
    BigEndian,
    LittleEndian = 1,
};

/*!
 * \brief Get current native endianness
 */
inline Endian getEndianess()
{
    union {
        uint32_t i;
        char c[4];
    } bint = { 0x01020304 };

    return bint.c[0] == 0x01 ? Endian::BigEndian : Endian::LittleEndian;
}

/*!
 * \brief Byte swap a integral number.
 */
template <typename T,
          typename = std::enable_if_t<std::is_integral_v<T>
                                      && sizeof(T) <= sizeof(unsigned long long)>>
inline T byteswap(T value)
{
    if constexpr (sizeof(T) == 1) {
        return value;
    }
    if constexpr (sizeof(T) == 2) {
        return (value >> 8) | (value << 8);
    }
    if constexpr (sizeof(T) == 4) {
        return (value >> 24) | ((value >> 8) & 0xFF00) | ((value << 8) & 0xFF0000) | (value << 24);
    }
    if constexpr (sizeof(T) == 8) {
        return (value >> 56) | ((value >> 40) & 0xFF00) | ((value >> 24) & 0xFF0000)
                | ((value >> 8) & 0xFF000000) | ((value << 8) & 0xFF00000000)
                | ((value << 24) & 0xFF0000000000) | ((value << 40) & 0xFF000000000000)
                | (value << 56);
    }
    return value;
}

/*!
 *  \brief Convert big endian to native endianness
 */
template <typename T,
          typename = std::enable_if_t<std::is_integral_v<T>
                                      && sizeof(T) <= sizeof(unsigned long long)>>
inline T beToNative(T value)
{
    auto endianess = getEndianess();
    if (endianess == Endian::BigEndian) {
        return value;
    }
    return byteswap(value);
}

/*!
 *  \brief Convert little endian to native endianness
 */
template <typename T,
          typename = std::enable_if_t<std::is_integral_v<T>
                                      && sizeof(T) <= sizeof(unsigned long long)>>
inline T leToNative(T value)
{
    auto endianess = getEndianess();
    if (endianess == Endian::LittleEndian) {
        return value;
    }
    return byteswap(value);
}
/*!
 *  \brief Convert native endianness to big endian
 */
template <typename T,
          typename = std::enable_if_t<std::is_integral_v<T>
                                      && sizeof(T) <= sizeof(unsigned long long)>>
inline constexpr T nativeToBe(T num)
{
    return beToNative<T>(num);
}

/*!
 *  \brief Convert native endianness to little endian
 */
template <typename T,
          typename = std::enable_if_t<std::is_integral_v<T>
                                      && sizeof(T) <= sizeof(unsigned long long)>>
inline constexpr T nativeToLe(T num)
{
    return leToNative<T>(num);
}

/*!
 * \brief Helper alias for string iterator(just minimize code size)
 */
template <class T>
using string_const_iterator = typename std::basic_string<T>::const_iterator;

/*!
 * \brief Convert string from one encoding to another using iconv
 */
template <typename target_char, typename source_char>
inline std::basic_string<target_char> convert(string_const_iterator<source_char> begin,
                                              string_const_iterator<source_char> end, iconv_t conv)
{
    std::basic_string<target_char> result = {};

    char *inbuf = reinterpret_cast<char *>(const_cast<source_char *>(&*begin));
    size_t inbytesLeft = std::distance(begin, end) * sizeof(source_char);

    auto temp = std::make_unique<std::array<char, 512>>();
    char *outbuf = temp->data();
    size_t outbytesLeft = temp->size();

    while (inbytesLeft > 0) {
        auto ret = iconv(conv, &inbuf, &inbytesLeft, &outbuf, &outbytesLeft);
        if (ret == ICONV_ERROR_CODE && errno != E2BIG) {
            throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                                     + ", Encountered corrupted unicode string.");
        }

        result.append(reinterpret_cast<target_char *>(temp->data()),
                      reinterpret_cast<target_char *>(outbuf));
        outbuf = temp->data();
        outbytesLeft = temp->size();
    }

    return result;
}

/*!
 * \brief Convert string from one encoding to another using iconv
 */
template <typename target_char, typename source_char>
inline std::basic_string<target_char> convert(const std::basic_string<source_char> &source,
                                              iconv_t conv)
{
    return convert<target_char, source_char>(source.cbegin(), source.cend(), conv);
}

} // namespace pol

#endif // PREGPARSER_ENCODING
