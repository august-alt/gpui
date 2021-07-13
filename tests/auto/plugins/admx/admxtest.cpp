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

#include "admxtest.h"

#include "../../../../src/io/policydefinitionsfile.h"
#include "../../../../src/model/admx/policy.h"
#include "../../../../src/model/admx/policyelement.h"
#include "../../../../src/plugins/admx/admxformat.h"

#include <fstream>

const std::string dataPath = "../../../data/";

using namespace io;
using namespace model::admx;

namespace tests {

void AdmxTest::read()
{
    gpui::AdmxFormat format;

    std::ifstream file;

    file.open (dataPath + "example.admx", std::ifstream::in);

    if (file.good()) {
        std::unique_ptr<io::PolicyDefinitionsFile> policies = std::make_unique<io::PolicyDefinitionsFile>();

        format.read(file, policies.get());
    }

    file.close();
}

}

QTEST_MAIN(tests::AdmxTest)
