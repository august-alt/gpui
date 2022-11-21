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

#ifndef SCRIPTS_TREE_PROXY_MODEL_H
#define SCRIPTS_TREE_PROXY_MODEL_H

#include <QIdentityProxyModel>

#include "../../../src/core/common.h"

#include "scriptsmodel.h"
#include "scriptssnapin.h"

namespace ModelView
{
class SessionModel;
class ViewModel;
} // namespace ModelView

class QItemSelectionModel;

namespace scripts_plugin
{
class ScriptsSnapIn;
}
namespace scripts_plugin
{
class ScriptsTreeProxyModelPrivate
{
public:
    ScriptsModel *userScriptsModel      = nullptr;
    ScriptsModel *userPowerScriptsModel = nullptr;

    ScriptsModel *machineScriptsModel      = nullptr;
    ScriptsModel *machinePowerScriptsModel = nullptr;

    ScriptsSnapIn *snapIn = nullptr;
};

class GPUI_SYMBOL_EXPORT ScriptsTreeProxyModel : public QIdentityProxyModel
{
    Q_OBJECT

public:
    ScriptsTreeProxyModel();

    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const override;

    void setTreeModel(ScriptsModel *userScripts,
                      ScriptsModel *userPowerScripts,
                      ScriptsModel *machineScripts,
                      ScriptsModel *machinePowerScripst);

    void setSnapIn(ScriptsSnapIn *scriptsSnapIn);

    ScriptsSnapIn *getSnapIn();

private:
    ModelView::SessionModel *sessionModel = nullptr;
    ModelView::ViewModel *viewModel       = nullptr;

    std::unique_ptr<ScriptsTreeProxyModelPrivate> d;

private:
    ScriptsTreeProxyModel(const ScriptsTreeProxyModel &) = delete;
    ScriptsTreeProxyModel(ScriptsTreeProxyModel &&)      = delete;
    ScriptsTreeProxyModel &operator=(const ScriptsTreeProxyModel &) = delete;
    ScriptsTreeProxyModel &operator=(const ScriptsTreeProxyModel &&) = delete;
};
} // namespace scripts_plugin

#endif //SCRIPTS_TREE_PROXY_MODEL_H
