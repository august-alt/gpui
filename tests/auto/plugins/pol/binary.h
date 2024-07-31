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
#ifndef PREGPARSER_TEST_BINARY
#define PREGPARSER_TEST_BINARY

#include "../../../../src/plugins/pol/binary.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

void testBufferToIntegral()
{
    std::stringstream buffer;
    char tmp[4] = { 0x12, 0x34, 0x56, 0x78 };
    const uint32_t &num = *reinterpret_cast<uint32_t *>(&tmp[0]);

    buffer.write(tmp, 4);
    buffer.seekg(0);

    std::optional<uint32_t> result = pol::bufferToIntegral<uint32_t, true>(buffer);

    assert(result.value_or(0) == num);

    std::cout << "bufferToIntegral<uint32_t, true>: OK" << std::endl;

    //----------------------------------------------//

    buffer.seekg(0);

    buffer.write(reinterpret_cast<const char *>(&num), 4);
    buffer.seekg(0);

    result = pol::bufferToIntegral<uint32_t, false>(buffer);

    std::reverse(tmp, tmp + 4);
    assert(result.value_or(0) == num);

    std::cout << "bufferToIntegral<uint32_t, false>: OK" << std::endl;
}

void testIntegralToBuffer() { }

void testBinary()
{
    testBufferToIntegral();
}

#endif // PREGPARSER_TEST_BINARY
