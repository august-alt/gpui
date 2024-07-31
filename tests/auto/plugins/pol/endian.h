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
#ifndef PREGPARSER_TEST_ENDIAN
#define PREGPARSER_TEST_ENDIAN

#include "../../../../src/plugins/pol/encoding.h"
#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <iostream>
#include <sstream>

void testEndian()
{

    uint8_t num1 = 0x12;
    assert(pol::byteswap<uint8_t>(num1) == 0x12);
    std::cout << "byteswap<uint8_t>: OK" << std::endl;

    uint16_t num2 = 0x1234;
    assert(pol::byteswap<uint16_t>(num2) == 0x3412);
    std::cout << "byteswap<uint16_t>: OK" << std::endl;

    uint32_t num3 = 0x12345678;
    assert(pol::byteswap<uint32_t>(num3) == 0x78563412);
    std::cout << "byteswap<uint32_t>: OK" << std::endl;

    uint64_t num4 = 0x123456789ABCDEF0;
    assert(pol::byteswap<uint64_t>(num4) == 0xF0DEBC9A78563412);
    std::cout << "byteswap<uint64_t>: OK" << std::endl;
}

#endif // PREGPARSER_TEST_ENDIAN
