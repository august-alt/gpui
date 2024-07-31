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
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "../../../../src/plugins/pol/parser.h"

const std::string dataPath = "../../../data/";

void testCase(std::string filename)
{
    std::ifstream file(dataPath + filename, std::ios::in | std::ios::binary);
    std::stringstream stream;

    auto parser = pol::createPregParser();
    auto pol = parser->parse(file);

    parser->write(stream, pol);
    auto pol2 = parser->parse(stream);

    if (pol != pol2) {
        std::cout << "`" << filename << "` is rewrite: FAIL" << std::endl;
        throw std::runtime_error("LINE: " + std::to_string(__LINE__) + ", FILE: " + __FILE__
                                 + ", Encountered with invalid parser/serializer work.");
    }

    std::cout << "`" << filename << "` is rewrite: OK" << std::endl;
}
