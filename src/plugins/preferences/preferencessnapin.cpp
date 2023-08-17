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

#include "preferencessnapin.h"

#include "preferencessnapinprivate.h"

#include <iostream>

#include "../../gui/mainwindow.h"

#include "interfaces/containeriteminterface.h"

#include "common/preferencesmodel.h"
#include "common/preferencestreemodel.h"
#include "common/preferencestreeproxymodel.h"

#include "modelcreator.h"
#include "modelwriter.h"

#include <mvvm/viewmodel/topitemsviewmodel.h>

#include <QCoreApplication>
#include "../../core/logger/log.h"
#include <QDirIterator>

namespace gpui
{
using namespace preferences;

PreferencesSnapIn::PreferencesSnapIn()
    : AbstractSnapIn("ISnapIn",
                     "PreferencesSnapIn",
                     "Snap-in for preferences management.",
                     {1, 0, 0},
                     "GPL-2.0",
                     "Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>")
    , d(new PreferencesSnapInPrivate())
{}

void PreferencesSnapIn::onInitialize(QMainWindow *window)
{
    auto mainWindow = dynamic_cast<MainWindow *>(window);

    if (mainWindow)
    {
        d->localeName = mainWindow->getLanguage().toStdString();
    }

    d->proxyViewModel = std::make_unique<PreferencesTreeProxyModel>();
    d->proxyViewModel->setPreferencesModels(d->machinePreferencesModels.get(), d->userPreferencesModels.get());
    d->proxyViewModel->setSnapIn(d.get());

    onRetranslateUI(d->localeName);

    setRootNode(d->proxyViewModel.get());

    if (mainWindow)
    {
        QObject::connect(d->proxyViewModel.get(),
                         &PreferencesTreeProxyModel::savePolicyChanges,
                         mainWindow,
                         &MainWindow::updateStatusBar);
    }

    std::cout << std::string(__PRETTY_FUNCTION__) << std::endl;
}

void PreferencesSnapIn::onShutdown()
{
    std::cout << std::string(__PRETTY_FUNCTION__) << std::endl;
}

void PreferencesSnapIn::onDataLoad(const std::string &policyPath, const std::string &locale)
{
    Q_UNUSED(locale);

    d->policyPath = policyPath;

    auto modelCreator = std::make_unique<ModelCreator>();

    modelCreator->populateModels(policyPath, "Machine", d->machinePreferencesModels.get());
    modelCreator->populateModels(policyPath, "User", d->userPreferencesModels.get());

    d->proxyViewModel->setPreferencesModels(d->machinePreferencesModels.get(), d->userPreferencesModels.get());
    d->proxyViewModel->setSnapIn(d.get());
}

void PreferencesSnapIn::onDataSave()
{
    d->onDataSave();
}

void PreferencesSnapIn::onRetranslateUI(const std::string &locale)
{
    d->model     = std::make_unique<PreferencesTreeModel>();
    d->viewModel = std::make_unique<ModelView::TopItemsViewModel>(d->model.get());
    d->proxyViewModel->setSourceModel(d->viewModel.get());

    d->retranslateModels(d->machinePreferencesModels);
    d->retranslateModels(d->userPreferencesModels);
}

} // namespace gpui
