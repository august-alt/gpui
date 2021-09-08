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

#include "preferencesdialogtest.h"

#include <fstream>

#include "../../../../../src/plugins/preferences/shortcuts/modelbuilder.h"
#include "../../../../../src/plugins/preferences/shortcuts/shortcutroles.h"
#include "../../../../../src/plugins/preferences/shortcuts/shortcutsschema.h"
#include "../../../../../src/plugins/preferences/shortcuts/shortcutswidget.h"

#include "../../../../../src/plugins/preferences/common/preferencesdialog.h"

const std::string dataPath = "../../../../data/";

namespace tests {

void PreferencesDialogTest::show()
{
    std::ifstream file;

    file.open (dataPath + "shortcuts.xml", std::ifstream::in);

    if (file.good()) {
       auto shortcuts = Shortcuts_(file, ::xsd::cxx::tree::flags::dont_validate);
       auto modelBuilder = std::make_unique<gpui::ModelBuilder>();
       auto model = modelBuilder->schemaToModel(shortcuts);

       auto selectionModel = std::make_unique<QItemSelectionModel>(model.get());

       auto widget = new gpui::ShortcutsWidget(*model, *selectionModel);
       selectionModel->setCurrentIndex(model->item(0, 12)->index(),
                                       QItemSelectionModel::Select | QItemSelectionModel::Rows);

       auto preferencesDialog = std::make_unique<gpui::PreferencesDialog>(widget);
       preferencesDialog->show();

       QTest::qWait(10000);
    }

    file.close();
}

}

QTEST_MAIN(tests::PreferencesDialogTest)
