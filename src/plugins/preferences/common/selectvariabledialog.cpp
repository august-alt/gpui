/***********************************************************************************************************************
**
** Copyright (C) 2024 BaseALT Ltd. <org@basealt.ru>
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

#include "selectvariabledialog.h"
#include "ui_selectvariabledialog.h"
#include "variablesmodel.h"

namespace preferences
{

SelectVariableDialog::SelectVariableDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectVariableDialog())
    , model(new VariablesModel(this))
{
    ui->setupUi(this);

    ui->variablesTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->variablesTableView->setModel(this->model.get());

    ui->variablesTableView->resizeColumnsToContents();

    this->setWindowTitle(tr("Select a variable"));

    connect(ui->okPushButton, &QPushButton::clicked, this, &SelectVariableDialog::onItemSelected);
    connect(ui->variablesTableView, &QTableView::doubleClicked, ui->okPushButton, &QPushButton::click);
}

SelectVariableDialog::~SelectVariableDialog()
{
}

void SelectVariableDialog::onItemSelected()
{
    auto selectedIndexes = ui->variablesTableView->selectionModel()->selectedIndexes();

    if (selectedIndexes.size() > 0)
    {
        this->variableSelected(QString("%%1%").arg(model->itemFromIndex(selectedIndexes[0])->text()));
    }
    else
    {
        this->variableSelected("%AppDataDir%");
    }
}

}
