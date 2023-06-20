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

#include "admltest.h"

#include "../../../../src/io/policyresourcesfile.h"
#include "../../../../src/plugins/administrative_templates/presentation/policyresources.h"
#include "../../../../src/plugins/administrative_templates/presentation/presentation.h"
#include "../../../../src/plugins/administrative_templates/presentation/presentationwidget.h"
#include "../../../../src/plugins/adml/admlformat.h"

#include "../../../../src/plugins/administrative_templates/ui/presentationbuilder.h"

#include "../../../../src/plugins/administrative_templates/admx/policy.h"
#include "../../../../src/plugins/administrative_templates/admx/policyelement.h"

#include "../../../../src/plugins/administrative_templates/commands/commandgroup.h"

#include "../../../../src/plugins/administrative_templates/registry/polregistrysource.h"
#include "../../../../src/plugins/administrative_templates/registry/registry.h"

#include <fstream>

#include <QVBoxLayout>

const std::string dataPath = "../../../data/";

using namespace io;
using namespace model::presentation;

namespace tests
{
void AdmlTest::read()
{
    gpui::AdmlFormat format;

    std::ifstream file;

    file.open(dataPath + "example.adml", std::ifstream::in);

    if (file.good())
    {
        std::unique_ptr<io::PolicyResourcesFile> policies = std::make_unique<io::PolicyResourcesFile>();

        format.read(file, policies.get());

        auto policyResource = policies->get(0);
        if (policyResource)
        {
            if (policyResource->presentationTable.size() > 0)
            {
                Presentation presentation = *policyResource->presentationTable.begin()->second;

                auto policy            = std::make_unique<model::admx::Policy>();
                auto registry          = std::make_shared<model::registry::Registry>();
                auto polRegistrySource = std::make_unique<model::registry::PolRegistrySource>(registry);
            }
        }
    }

    file.close();
}

} // namespace tests

QTEST_MAIN(tests::AdmlTest)
