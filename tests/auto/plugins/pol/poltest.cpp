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
#include "generatecase.h"
#include "../../../../src/plugins/pol/binary.h"
#include "../../../../src/plugins/pol/encoding.h"
#include "../../../../src/plugins/pol/parser.h"

#include <fstream>
#include <iostream>
#include <sstream>

const std::string dataPath = "../../../data/";

namespace tests {

void PolTest::endianness()
{
    uint8_t num1 = 0x12;
    QCOMPARE(pol::byteswap<uint8_t>(num1), 0x12);

    uint16_t num2 = 0x1234;
    QCOMPARE(pol::byteswap<uint16_t>(num2), 0x3412);
    qDebug() << "byteswap<uint16_t>: OK";

    uint32_t num3 = 0x12345678;
    QCOMPARE(pol::byteswap<uint32_t>(num3), 0x78563412);
    qDebug() << "byteswap<uint32_t>: OK";

    uint64_t num4 = 0x123456789ABCDEF0;
    QCOMPARE(pol::byteswap<uint64_t>(num4), 0xF0DEBC9A78563412);
    qDebug() << "byteswap<uint64_t>: OK";
}
void PolTest::bufferToIntegral()
{
    std::stringstream buffer;
    char tmp[4] = { 0x12, 0x34, 0x56, 0x78 };
    const uint32_t &num = *reinterpret_cast<uint32_t *>(&tmp[0]);

    buffer.write(tmp, 4);
    buffer.seekg(0);

    uint32_t result = pol::bufferToIntegral<uint32_t, true>(buffer);

    QCOMPARE(result, num);

    qDebug() << "bufferToIntegral<uint32_t, true>: OK";

    //----------------------------------------------//

    buffer.seekg(0);

    buffer.write(reinterpret_cast<const char *>(&num), 4);
    buffer.seekg(0);

    result = pol::bufferToIntegral<uint32_t, false>(buffer);

    std::reverse(tmp, tmp + 4);
    QCOMPARE(result, num);

    qDebug() << "bufferToIntegral<uint32_t, false>: OK";
}

void PolTest::testCase_data()
{
    QTest::addColumn<QString>("filename");

    QTest::newRow("case1") << "case1.pol";
    QTest::newRow("case2") << "case2.pol";
}

void PolTest::autogenerateCases_data()
{
    QTest::addColumn<uint64_t>("seed");

    std::mt19937 gen;
    std::random_device dev;
    size_t seed = dev();

    gen.seed(seed);

    for (size_t i = 0; i < 50; ++i) {
        std::string name = std::to_string(i) + ". seed " + std::to_string(seed);
        QTest::newRow(name.c_str()) << seed;
    }
}

void PolTest::testCase(QString filename)
{
    auto stdFilename = filename.toStdString();

    std::ifstream file(dataPath + stdFilename, std::ios::in | std::ios::binary);
    std::stringstream stream;

    auto parser = pol::createPregParser();
    auto pol = parser->parse(file);

    parser->write(stream, pol);
    auto pol2 = parser->parse(stream);

    auto failMessage = "`" + stdFilename + "` << is rewrite failed";
    QVERIFY2(pol == pol2, failMessage.c_str());
}

void PolTest::autogenerateCases(size_t seed)
{
    pol::PolicyFile policyFile = generateCase(seed);
    std::stringstream file;
    auto parser = pol::createPregParser();

    parser->write(file, policyFile);
    file.seekg(0, std::ios::beg);

    auto test = parser->parse(file);
    QCOMPARE(policyFile, test);
}
} // namespace tests

QTEST_MAIN(tests::PolTest)
