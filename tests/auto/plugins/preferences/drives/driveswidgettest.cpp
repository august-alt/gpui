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

#include "driveswidgettest.h"

#include <fstream>

#include "../../../../../src/plugins/preferences/drives/modelbuilder.h"
#include "../../../../../src/plugins/preferences/drives/driveroles.h"
#include "../../../../../src/plugins/preferences/drives/drivesschema.h"
#include "../../../../../src/plugins/preferences/drives/driveswidget.h"

const std::string dataPath = "../../../../data/";

namespace tests {

void DrivesWidgetTest::show()
{
    std::ifstream file;

    file.open(dataPath + "drives.xml", std::ifstream::in);
    if (file.good()) {
       auto drives = Drives_(file, ::xsd::cxx::tree::flags::dont_validate);
       auto modelBuilder = std::make_unique<gpui::ModelBuilder>();
       auto model = modelBuilder->schemaToModel(drives);

       auto selectionModel = std::make_unique<QItemSelectionModel>(model.get());

       auto widget = std::make_unique<gpui::DrivesWidget>(*model, *selectionModel);
       widget->show();

       selectionModel->setCurrentIndex(model->item(0, 1)->index(),
                                       QItemSelectionModel::Select | QItemSelectionModel::Rows);

       QTest::qWait(10000);
    }

    file.close();
}

}

QTEST_MAIN(tests::DrivesWidgetTest)
