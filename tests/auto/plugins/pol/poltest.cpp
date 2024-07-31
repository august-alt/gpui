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
void PolTest::read()
{
    testEndian();
    testBinary();
    testCase("case1.pol");
    testCase("case2.pol");
    generateCase(100);
    /*
        gpui::PolFormat format;

        std::ifstream file;

        file.open(dataPath + "example.pol", std::ifstream::in);

        if (file.good()) {
            std::unique_ptr<io::RegistryFile> registry = std::make_unique<io::RegistryFile>();

            format.read(file, registry.get());

            for (auto &entry : registry->getRegistry()->registryEntries) {
                if (entry) {
                    std::cout << "Key name " << entry->key.toStdString() << std::endl;
                    std::cout << "Value name " << entry->value.toStdString() << std::endl;
                    std::cout << "Type " << entry->type << std::endl;
                }
            }
        }

        file.close();
    */
}

} // namespace tests

QTEST_MAIN(tests::PolTest)
