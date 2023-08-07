/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
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

#include "scriptssnapin.h"
#include "../../io/genericreader.h"
#include "../../io/inifile.h"
#include "../../plugins/storage/smb/smbfile.h"
#include "gui/mainwindow.h"

#include "scriptmodelbuilder.h"
#include "scriptsmodel.h"
#include "scriptsmodelio.h"
#include "scriptstreemodel.h"
#include "scriptstreeproxymodel.h"

#include <mvvm/viewmodel/defaultviewmodel.h>
#include <mvvm/viewmodel/propertytableviewmodel.h>
#include <mvvm/viewmodel/topitemsviewmodel.h>

#include <../../io/genericwriter.h>
#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/model/sessionmodel.h>
#include <QDebug>
#include <QFile>

namespace scripts_plugin
{
ScriptsSnapIn::ScriptsSnapIn()
    : AbstractSnapIn("ISnapIn",
                     "ScriptsSnapin",
                     "SnapIn for Scripts management",
                     {1, 0, 0},
                     "GPL-2.0",
                     "Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru")
    , d(new ScriptsSnapInPrivate(this))
{}

ScriptsSnapIn::~ScriptsSnapIn()
{
    delete d;
}

void ScriptsSnapIn::onInitialize(QMainWindow *window)
{
    auto mainWindow = dynamic_cast<gpui::MainWindow *>(window);

    if (mainWindow)
    {
        d->localeName = mainWindow->getLanguage().toStdString();
    }

    d->proxyViewModel->setSourceModel(d->viewModel.get());

    d->proxyViewModel->setSnapIn(this);

    onRetranslateUI(d->localeName);

    setRootNode(d->proxyViewModel.get());

    if (mainWindow)
    {}
}

void ScriptsSnapIn::onShutdown() {}

void ScriptsSnapIn::onDataLoad(const std::string &policyPath, const std::string &locale)
{
    d->localeName = locale;

    if (policyPath.empty())
    {
        qWarning() << "Warning: Unable to load ini file. Path is empty! ";

        return;
    }

    d->policyPath = policyPath;

    if (policyPath.back() != '\\' && policyPath.back() != '/')
    {
        d->policyPath.append("/");
    }

    d->modelIo.get()->loadPolicies(d->policyPath,
                                   d->userScriptsModel.get(),
                                   d->userPowerScriptsModel.get(),
                                   d->machineScriptsModel.get(),
                                   d->machinePowerScriptsModel.get());
}

void ScriptsSnapIn::onDataSave()
{
    d->modelIo.get()->savePolicies(d->policyPath,
                                   d->userScriptsModel.get(),
                                   d->userPowerScriptsModel.get(),
                                   d->machineScriptsModel.get(),
                                   d->machinePowerScriptsModel.get());
}

void ScriptsSnapIn::onRetranslateUI(const std::string &locale)
{
    Q_UNUSED(locale);

    d->treeModel      = std::make_unique<ScriptsTreeModel>();
    d->viewModel      = ModelView::Factory::CreateTopItemsViewModel(d->treeModel.get());
    d->proxyViewModel = std::make_unique<ScriptsTreeProxyModel>();
    d->proxyViewModel->setSourceModel(d->viewModel.get());

    setRootNode(d->proxyViewModel.get());

    d->proxyViewModel->setSnapIn(this);

    d->retranslateModels(d->userScriptsModel);
    d->retranslateModels(d->userPowerScriptsModel);

    d->retranslateModels(d->machineScriptsModel);
    d->retranslateModels(d->machinePowerScriptsModel);
}

} // namespace scripts_plugin
