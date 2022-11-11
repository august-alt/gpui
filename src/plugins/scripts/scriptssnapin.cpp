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
#include "../../plugins/storage/smb/smbfile.h"
#include "../../io/genericreader.h"
#include "../../io/inifile.h"
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
class ScriptsSnapInPrivate
{
public:
    std::unique_ptr<std::string> policyPath                        = nullptr;
    std::unique_ptr<ScriptsModel> userScriptsModel                 = nullptr;
    std::unique_ptr<ScriptsModel> userPowerScriptsModel            = nullptr;
    std::unique_ptr<ScriptsModel> machineScriptsModel              = nullptr;
    std::unique_ptr<ScriptsModel> machinePowerScriptsModel         = nullptr;
    std::unique_ptr<ModelView::SessionModel> treeModel             = nullptr;
    std::unique_ptr<ModelView::ViewModel> viewModel                = nullptr;
    std::unique_ptr<ScriptsTreeProxyModel> proxyViewModel = nullptr;
    std::unique_ptr<ScriptsModelIo> modelIo                        = nullptr;

public:
    ScriptsSnapInPrivate() {}

private:
    ScriptsSnapInPrivate(const ScriptsSnapInPrivate &) = delete;      //copy ctor
    ScriptsSnapInPrivate(ScriptsSnapInPrivate &&)      = delete;      //move ctor
    ScriptsSnapInPrivate operator=(ScriptsSnapInPrivate &) = delete;  //copy assignment
    ScriptsSnapInPrivate operator=(ScriptsSnapInPrivate &&) = delete; //move assignment
};

ScriptsSnapIn::ScriptsSnapIn()
    : AbstractSnapIn("ISnapIn",
                     "PreferencesSnapin",
                     "SnapIn for Scripts management",
                     {1, 0, 0},
                     "GPL-2.0",
                     "Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru")
    , d(new ScriptsSnapInPrivate())
{}

ScriptsSnapIn::~ScriptsSnapIn()
{
    delete d;
}

void ScriptsSnapIn::onInitialize(QMainWindow *mainWindow)
{
    d->userScriptsModel         = std::make_unique<ScriptsModel>();
    d->userPowerScriptsModel    = std::make_unique<ScriptsModel>();
    d->machineScriptsModel      = std::make_unique<ScriptsModel>();
    d->machinePowerScriptsModel = std::make_unique<ScriptsModel>();
    d->treeModel                = std::make_unique<ScriptsTreeModel>();
    d->viewModel                = ModelView::Factory::CreateTopItemsViewModel(d->treeModel.get());
    d->proxyViewModel           = std::make_unique<ScriptsTreeProxyModel>();
    d->proxyViewModel->setSourceModel(d->viewModel.get());
    d->proxyViewModel->setTreeModel({d->treeModel.get(),
                                     d->viewModel.get(),
                                     d->machineScriptsModel.get(),
                                     d->userScriptsModel.get(),
                                     this});
    d->modelIo = std::make_unique<ScriptsModelIo>();
}

void ScriptsSnapIn::onShutdown() {}

void ScriptsSnapIn::onDataLoad(const std::string &policyPath, const std::string &locale)
{
    if (policyPath.empty())
    {
        qWarning() << "Warning: Unable to load ini file. Path is empty! ";

        return;
    }

    d->policyPath = std::make_unique<std::string>(policyPath);

    d->modelIo.get()->loadPolicies(policyPath,
                                   d->userScriptsModel.get(),
                                   d->userPowerScriptsModel.get(),
                                   d->machineScriptsModel.get(),
                                   d->machinePowerScriptsModel.get());

    d->proxyViewModel->setMachineModel(d->machineScriptsModel.get());
    d->proxyViewModel->setUserModel(d->userScriptsModel.get());
}

void ScriptsSnapIn::onDataSave()
{
    d->modelIo.get()->savePolicies(*d->policyPath.get(),
                                   d->userScriptsModel.get(),
                                   d->userPowerScriptsModel.get(),
                                   d->machineScriptsModel.get(),
                                   d->machinePowerScriptsModel.get());
}

void ScriptsSnapIn::onRetranslateUI(const std::string &locale)
{
    Q_UNUSED(locale);
}

QAbstractItemModel *ScriptsSnapIn::getRootNode() const
{
    return d->proxyViewModel.get();
}
} // namespace scripts_plugin
