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

#include "poltest.h"
#include "binary.h"
#include "endian.h"
#include "generatecase.h"

#include <fstream>
#include <iostream>

namespace tests {

void PolTest::endianness()
{
    testEndian();
}
void PolTest::binary()
{
    testBinary();
}
void PolTest::case1()
{
    testCase("case1.pol");
}
void PolTest::case2()
{
    testCase("case2.pol");
}
void PolTest::autogenerateCases()
{
    generateCase(100);
}
} // namespace tests

QTEST_MAIN(tests::PolTest)
