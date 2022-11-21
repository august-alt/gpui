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

#include "scriptsdialog.h"
#include "groupscriptcontaineritem.h"
#include "scriptitemcontainer.h"
#include "ui_scriptsdialog.h"
#include "ui_scriptspowershellwidget.h"
#include "ui_scriptswidget.h"

#include <mvvm/factories/viewmodelfactory.h>

#include <QDebug>

namespace scripts_plugin
{
ScriptsDialog::ScriptsDialog(ModelView::SessionItem *scriptsItem,
                             ModelView::SessionItem *psScriptsItem,
                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ScriptsDialog())

{
    ui->setupUi(this);

    setItem(scriptsItem, ui->scriptsTab);
    setItem(psScriptsItem, ui->powerShellScriptsTab);
}

ScriptsDialog::ScriptsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ScriptsDialog())
{
    ui->setupUi(this);
}

ScriptsDialog::~ScriptsDialog()
{
    delete ui;
}

void ScriptsDialog::setModels(ScriptsModel *scriptsModel, ScriptsModel *powerScriptsModel)
{
    auto scriptsItem      = scriptsModel->topItem<ScriptItemContainer>();
    auto powerScriptsItem = powerScriptsModel->topItem<ScriptItemContainer>();

    setItem(scriptsItem, ui->scriptsTab);
    setItem(powerScriptsItem, ui->powerShellScriptsTab);
}

template<typename TWidget>
void ScriptsDialog::setItem(ModelView::SessionItem *item, TWidget &widget)
{
    auto scriptsItem = dynamic_cast<ScriptItemContainer *>(item);
    if (scriptsItem)
    {
        widget->setItem(scriptsItem->getScripts());
    }
}

void ScriptsDialog::submit() {}

void ScriptsDialog::on_buttonBox_accepted()
{
    emit(saveDataSignal());
}

void ScriptsDialog::on_buttonBox_rejected()
{
    emit(reloaddataSignal());
}

} // namespace scripts_plugin
