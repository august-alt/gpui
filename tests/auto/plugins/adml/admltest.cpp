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

#include "admltest.h"

#include "../../../../src/io/policyresourcesfile.h"
#include "../../../../src/model/presentation/policyresources.h"
#include "../../../../src/model/presentation/presentationwidget.h"
#include "../../../../src/model/presentation/presentation.h"
#include "../../../../src/plugins/adml/admlformat.h"

#include "../../../../src/gui/presentationbuilder.h"

#include <fstream>

const std::string dataPath = "../../../data/";

using namespace io;
using namespace model::presentation;

namespace tests {

void AdmlTest::read()
{
    gpui::AdmlFormat format;

    std::ifstream file;

    file.open (dataPath + "example.adml", std::ifstream::in);

    if (file.good())
    {
        std::unique_ptr<io::PolicyResourcesFile> policies = std::make_unique<io::PolicyResourcesFile>();

        format.read(file, policies.get());


        auto policyResource = policies->get(0);
        if (policyResource) {
            Presentation presentation;
            presentation.widgets = policyResource->presentationTable;

            auto widget = gui::PresentationBuilder::build(presentation);
            widget->show();

            QTest::qWait(10000);
        }
    }

    file.close();
}

}

QTEST_MAIN(tests::AdmlTest)
