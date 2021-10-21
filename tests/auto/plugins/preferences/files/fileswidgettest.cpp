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

#include "fileswidgettest.h"

#include <fstream>

#include "../../../../../src/plugins/preferences/files/modelbuilder.h"
#include "../../../../../src/plugins/preferences/files/fileroles.h"
#include "../../../../../src/plugins/preferences/files/filesschema.h"
#include "../../../../../src/plugins/preferences/files/fileswidget.h"

const std::string dataPath = "../../../../data/";

namespace tests {

void FilesWidgetTest::show()
{
    std::ifstream file;

    file.open(dataPath + "files.xml", std::ifstream::in);
    if (file.good()) {
       auto modelSource = Files_(file, ::xsd::cxx::tree::flags::dont_validate);
       auto modelBuilder = std::make_unique<gpui::ModelBuilder>();
       auto model = modelBuilder->schemaToModel(modelSource);

       auto selectionModel = std::make_unique<QItemSelectionModel>(model.get());

       auto widget = std::make_unique<gpui::FilesWidget>(*model, *selectionModel);
       widget->show();

       selectionModel->setCurrentIndex(model->item(0, 0)->index(),
                                       QItemSelectionModel::Select | QItemSelectionModel::Rows);

       QTest::qWait(10000);
    }

    file.close();
}

}

QTEST_MAIN(tests::FilesWidgetTest)
