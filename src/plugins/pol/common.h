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

#ifndef PREGPARSER_COMMON
#define PREGPARSER_COMMON

#include <iostream>
#include <limits>

#include "encoding.h"

#include "iconv.h"

namespace pol 
{
inline void check_stream(std::istream &target)
{
    if (target.fail()) {
        if (target.eof()) {
            throw std::runtime_error("LINE: " + std::to_string(__LINE__)
                                     + ", FILE: " + __FILE__
                                     + ", Failed to read buffer, EOF was encountered.");
        }
        throw std::runtime_error("LINE: " + std::to_string(__LINE__)
                                    + ", FILE: " + __FILE__
                                    + ", Failed to read buffer, error was encountered.");
    }
}
inline void check_stream(std::ostream &target)
{
    if (target.fail()) {
        throw std::runtime_error("LINE: " + std::to_string(__LINE__)
                                    + ", FILE: " + __FILE__
                                    + ", Failed to write buffer, error was encountered.");
    }
}
inline void check_sym(std::istream &target, char16_t sym)
{
    char16_t buff;
    target.read(reinterpret_cast<char *>(&buff), 2);
    buff = leToNative(buff);

    check_stream(target);

    if (buff != sym) {
        throw std::runtime_error("LINE: " + std::to_string(__LINE__)
                                    + ", FILE: " + __FILE__
                                    + ", Failed to read/write buffer, invalid symbol was encountered.");
    }
}
inline void write_sym(std::ostream &target, char16_t sym)
{
    sym = nativeToLe(sym);
    target.write(reinterpret_cast<char *>(&sym), 2);

    check_stream(target);
}
} // namespace pol

#endif // PREGPARSER_COMMON
