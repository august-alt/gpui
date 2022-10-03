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

#include <iostream>

#include "common/preferencesmodel.h"
#include "preferencestreemodel.h"
#include "preferencestreeproxymodel.h"

#include "modelcreator.h"

#include <mvvm/viewmodel/topitemsviewmodel.h>

namespace gpui
{
using namespace preferences;

class PreferencesSnapInPrivate
{
    typedef std::unique_ptr<PreferencesModel> PreferencesModelPtr;
    typedef std::map<std::string, PreferencesModelPtr> PreferencesModelMap;

public:
    std::unique_ptr<PreferencesTreeModel> model                   = nullptr;
    std::unique_ptr<ModelView::ViewModel> viewModel               = nullptr;
    std::unique_ptr<QAbstractItemModel> proxyViewModel            = nullptr;
    std::unique_ptr<PreferencesModelMap> machinePreferencesModels = nullptr;
    std::unique_ptr<PreferencesModelMap> userPreferencesModels    = nullptr;

public:
    PreferencesSnapInPrivate() {}

private:
    PreferencesSnapInPrivate(const PreferencesSnapInPrivate &) = delete;            // copy ctor
    PreferencesSnapInPrivate(PreferencesSnapInPrivate &&)      = delete;            // move ctor
    PreferencesSnapInPrivate &operator=(const PreferencesSnapInPrivate &) = delete; // copy assignment
    PreferencesSnapInPrivate &operator=(PreferencesSnapInPrivate &&) = delete;      // move assignment
};

PreferencesSnapIn::PreferencesSnapIn()
    : AbstractSnapIn("ISnapIn",
                     "PreferencesSnapIn",
                     "Snap-in for preferences management.",
                     {1, 0, 0},
                     "GPL-2.0",
                     "Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>")
    , d(new PreferencesSnapInPrivate())
{}

void PreferencesSnapIn::onInitialize()
{
    d->model = std::make_unique<PreferencesTreeModel>();

    auto proxyModel = std::make_unique<PreferencesTreeProxyModel>();

    d->viewModel = std::make_unique<ModelView::TopItemsViewModel>(d->model.get());
    proxyModel->setSourceModel(d->viewModel.get());

    setRootNode(proxyModel.get());

    d->proxyViewModel = std::move(proxyModel);

    std::cout << std::string(__PRETTY_FUNCTION__) << std::endl;
}

void PreferencesSnapIn::onShutdown()
{
    std::cout << std::string(__PRETTY_FUNCTION__) << std::endl;
}

void PreferencesSnapIn::onDataLoad(const std::string &policyPath, const std::string &locale)
{
    Q_UNUSED(locale);

    auto modelCreator = std::make_unique<ModelCreator>();

    modelCreator->populateModels(policyPath, "Machine", d->machinePreferencesModels.get());
    modelCreator->populateModels(policyPath, "User", d->userPreferencesModels.get());
}

void PreferencesSnapIn::onDataSave() {}

void PreferencesSnapIn::onRetranslateUI(const std::string &locale)
{
    Q_UNUSED(locale);
}

} // namespace gpui
