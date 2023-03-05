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

#include "scriptscontentwidget.h"
#include "ui_scriptscontentwidget.h"

#include "scriptsdialog.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/viewitem.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace scripts_plugin
{
ScriptsContentWidget::ScriptsContentWidget(ScriptsSnapIn *sn, QWidget *parent)
    : QWidget(parent)
    , model(std::make_unique<QStandardItemModel>())
    , ui(new Ui::ScriptsContentWidget())
    , snapIn(sn)
{
    ui->setupUi(this);

    buildModel();
}

ScriptsContentWidget::~ScriptsContentWidget()
{
    delete ui;
}

void ScriptsContentWidget::setNamespace(bool machineNamespace)
{
    isMachineNamespace = machineNamespace;

    buildModel();
}

void ScriptsContentWidget::buildModel()
{
    model = std::make_unique<QStandardItemModel>();

    QString startupItem  = tr("Logon");
    QString shutdownItem = tr("Logoff");

    if (isMachineNamespace)
    {
        startupItem  = tr("Startup");
        shutdownItem = tr("Shutdown");
    }

    QStandardItem *first  = new QStandardItem();
    QStandardItem *second = new QStandardItem();

    first->setData(QVariant(startupItem), Qt::DisplayRole);
    second->setData(QVariant(shutdownItem), Qt::DisplayRole);
    first->setData(QVariant(true));
    second->setData(QVariant(false));

    model.get()->appendRow(first);
    model.get()->appendRow(second);

    ui->listView->setModel(model.get());

    connect(ui->listView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this,
            SLOT(startDialog(QItemSelection)));
}

void ScriptsContentWidget::startDialog(QItemSelection item)
{
    if (!item.indexes().isEmpty())
    {
        auto isStartupItem = model.get()->data(item.indexes().first(), isStartupRole);

        if (isStartupItem.toBool())
        {
            isStartupScripts = true;
        }
        else
        {
            isStartupScripts = false;
        }

        auto dialog = new ScriptsDialog(this);

        if (isMachineNamespace)
        {
            dialog->setModels(snapIn->d->machineScriptsModel.get(),
                              snapIn->d->machinePowerScriptsModel.get(),
                              isStartupScripts);
        }
        else
        {
            dialog->setModels(snapIn->d->userScriptsModel.get(),
                              snapIn->d->userPowerScriptsModel.get(),
                              isStartupScripts);
        }

        QObject::connect(dialog,
                         &ScriptsDialog::saveDataSignal,
                         snapIn->d,
                         &ScriptsSnapInPrivate::saveData);

        QObject::connect(dialog,
                         &ScriptsDialog::reloaddataSignal,
                         snapIn->d,
                         &ScriptsSnapInPrivate::reloadData);

        dialog->exec();
    }
}

} // namespace scripts_plugin
