/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
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

#include "policyfiletest.h"

#include "../../../../src/io/policydefinitionsfile.h"
#include "../../../../src/model/admx/policydefinitions.h"
#include "../../../../src/model/admx/policyelement.h"

using namespace io;
using namespace model::admx;

namespace tests {

void PolciyFileTest::getPolicy()
{
    PolicyDefinitionsFile file;

    auto policyA = std::make_shared<PolicyDefinitions>();
    auto policyB = std::make_shared<PolicyDefinitions>();
    auto policyC = std::make_shared<PolicyDefinitions>();

    file.addPolicyDefinitions(policyA);
    file.addPolicyDefinitions(policyB);
    file.addPolicyDefinitions(policyC);

    QVERIFY(file.getPolicyDefinitions(0) == policyA);
    QVERIFY(file.getPolicyDefinitions(1) == policyB);
    QVERIFY(file.getPolicyDefinitions(2) == policyC);
    QVERIFY(file.getPolicyDefinitions(3) == nullptr);
}

void PolciyFileTest::policyDefinitionsCount()
{
    PolicyDefinitionsFile file;

    QVERIFY(file.policyDefinitionsCount() == 0);

    auto policy = std::make_shared<PolicyDefinitions>();

    file.addPolicyDefinitions(policy);
    QVERIFY(file.policyDefinitionsCount() == 1);


    file.removePolicyDefinitions(policy);
    QVERIFY(file.policyDefinitionsCount() == 0);
}

}

QTEST_MAIN(tests::PolciyFileTest)
