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
#ifndef GPUI_PREFERENCES_SNAP_IN_PRIVATE_H
#define GPUI_PREFERENCES_SNAP_IN_PRIVATE_H

#include <map>
#include <memory>

#include <string>

#include <QObject>

#include "common/preferencesmodel.h"
#include "preferencestreemodel.h"
#include "preferencestreeproxymodel.h"

#include "modelcreator.h"
#include "modelwriter.h"

#include <mvvm/viewmodel/topitemsviewmodel.h>

namespace gpui
{
class PreferencesSnapInPrivate : public QObject
{
public:
    Q_OBJECT

private:
    typedef std::unique_ptr<preferences::PreferencesModel> PreferencesModelPtr;
    typedef std::map<std::string, PreferencesModelPtr> PreferencesModelMap;

public:
    std::unique_ptr<preferences::PreferencesTreeModel> model               = nullptr;
    std::unique_ptr<ModelView::ViewModel> viewModel                        = nullptr;
    std::unique_ptr<preferences::PreferencesTreeProxyModel> proxyViewModel = nullptr;
    std::unique_ptr<PreferencesModelMap> machinePreferencesModels          = nullptr;
    std::unique_ptr<PreferencesModelMap> userPreferencesModels             = nullptr;

    std::string policyPath{};

public:
    PreferencesSnapInPrivate();

public slots:
    void onDataSave();

private:
    PreferencesSnapInPrivate(const PreferencesSnapInPrivate &) = delete;            // copy ctor
    PreferencesSnapInPrivate(PreferencesSnapInPrivate &&)      = delete;            // move ctor
    PreferencesSnapInPrivate &operator=(const PreferencesSnapInPrivate &) = delete; // copy assignment
    PreferencesSnapInPrivate &operator=(PreferencesSnapInPrivate &&) = delete;      // move assignment
};
} // namespace gpui

#endif // GPUI_PREFERENCES_SNAP_IN_PRIVATE_H
