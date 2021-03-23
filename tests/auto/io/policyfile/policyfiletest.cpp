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

#include "../../../../src/io/policyfile.h"
#include "../../../../src/model/admx/policy.h"
#include "../../../../src/model/admx/policyelement.h"

using namespace io;
using namespace model::admx;

namespace tests {

void PolciyFileTest::getPolicy()
{
    PolicyFile file;

    auto policyA = std::make_shared<Policy>();
    auto policyB = std::make_shared<Policy>();
    auto policyC = std::make_shared<Policy>();

    file.addPolicy(policyA);
    file.addPolicy(policyB);
    file.addPolicy(policyC);

    QVERIFY(file.getPolicy(0) == policyA);
    QVERIFY(file.getPolicy(1) == policyB);
    QVERIFY(file.getPolicy(2) == policyC);

    policyA->name = "policyA";
    policyB->name = "policyB";
    policyC->name = "policyC";

    QVERIFY(file.getPolicy("policyC") == policyC);
    QVERIFY(file.getPolicy("policyA") == policyA);
    QVERIFY(file.getPolicy("policyB") == policyB);
    QVERIFY(file.getPolicy("no-policy") == nullptr);
}

void PolciyFileTest::contains()
{
    PolicyFile file;

    QVERIFY(file.policyCount() == 0);

    auto policy = std::make_shared<Policy>();
    policy->name = "policy";

    file.addPolicy(policy);

    QVERIFY(file.policyCount() == 1);
    QVERIFY(file.contains("policy"));

    file.removePolicy(policy);

    QVERIFY(file.policyCount() == 0);
    QVERIFY(file.contains("policy") == false);
}

}

QTEST_MAIN(tests::PolciyFileTest)
