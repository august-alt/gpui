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

#include "scriptstreeproxymodel.h"

#include "scriptscontentwidget.h"
#include "scriptsdialog.h"
#include "scriptssnapin.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <memory>

#include <QIcon>
#include <QUuid>

#include <QDebug>

namespace scripts_plugin
{
ScriptsTreeProxyModel::ScriptsTreeProxyModel()
    : d(std::make_unique<ScriptsTreeProxyModelPrivate>())
{}

QVariant ScriptsTreeProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    static QVariant folder = QVariant(QIcon::fromTheme("folder"));

    static QVariant computer = QVariant(QIcon::fromTheme("computer"));

    static QVariant user = QVariant(QIcon::fromTheme("user-home"));

    if (role == Qt::DecorationRole)
    {
        if (proxyIndex.data(Qt::DisplayRole).toString().compare("Machine", Qt::CaseInsensitive) == 0)
        {
            return computer;
        }

        if (proxyIndex.data(Qt::DisplayRole).toString().compare("User", Qt::CaseInsensitive) == 0)
        {
            return user;
        }

        return folder;
    }

    if (role == Qt::UserRole + 12)
    {
        auto viewModelProxy = static_cast<const ModelView::ViewModel *>(proxyIndex.model());
        auto item           = viewModelProxy->sessionItemFromIndex(proxyIndex);
        return QVariant::fromValue(item->property<QUuid>("NODE_ID"));
    }

    if (role == Qt::UserRole + 13)
    {
        auto viewModelProxy = static_cast<const ModelView::ViewModel *>(proxyIndex.model());
        return QVariant::fromValue(
            viewModelProxy->sessionItemFromIndex(proxyIndex)->property<QUuid>("PARENT_ID"));
    }

    if (role == 264)
    {
        std::function<QDialog *()> dialogCreator = [=]() {
            auto dialog = new ScriptsDialog();

            auto sessionItem = this->viewModel->sessionItemFromIndex(proxyIndex);

            auto name = sessionItem->displayName();

            if (sessionItem->displayName().compare("On startup") == 0
                || sessionItem->displayName().compare("On shutdown") == 0)
            {
                bool isStartupScripts = false;

                if (sessionItem->displayName().compare("On startup") == 0)
                    isStartupScripts = true;

                auto nameSpace = sessionItem->property<std::string>("NAMESPACE");
                if (nameSpace.compare("Machine") == 0)
                {
                    dialog->setModels(d->machineScriptsModel,
                                      d->machinePowerScriptsModel,
                                      isStartupScripts);
                }
                else
                {
                    dialog->setModels(d->userScriptsModel,
                                      d->userPowerScriptsModel,
                                      isStartupScripts);
                }
            }

            QObject::connect(dialog,
                             &ScriptsDialog::saveDataSignal,
                             d->snapIn->d,
                             &ScriptsSnapInPrivate::saveData);

            QObject::connect(dialog,
                             &ScriptsDialog::reloaddataSignal,
                             d->snapIn->d,
                             &ScriptsSnapInPrivate::reloadData);

            return dialog;
        };

        return QVariant::fromValue(dialogCreator);
    }

    if (role == 257)
    {
        auto sessionItem = this->viewModel->sessionItemFromIndex(proxyIndex);

        auto name = sessionItem->displayName();

        if (sessionItem->displayName().compare("On startup") == 0
            || sessionItem->displayName().compare("On shutdown") == 0)
        {
            return QVariant(static_cast<uint>(2));
        }
    }

    return QIdentityProxyModel::data(proxyIndex, role);
}

void ScriptsTreeProxyModel::setTreeModel(ScriptsModel *userScripts,
                                         ScriptsModel *userPowerScripts,
                                         ScriptsModel *machineScripts,
                                         ScriptsModel *machinePowerScripst)
{
    d->userScriptsModel         = userScripts;
    d->userPowerScriptsModel    = userPowerScripts;
    d->machineScriptsModel      = machineScripts;
    d->machinePowerScriptsModel = machinePowerScripst;
}

void ScriptsTreeProxyModel::setSnapIn(ScriptsSnapIn *scriptsSnapIn)
{
    if (scriptsSnapIn != nullptr)
    {
        d->snapIn = scriptsSnapIn;
    }
}

ScriptsSnapIn *ScriptsTreeProxyModel::getSnapIn()
{
    return d->snapIn;
}

} // namespace scripts_plugin

Q_DECLARE_METATYPE(std::shared_ptr<void(QWidget *, QItemSelectionModel *, const QModelIndex &)>)
Q_DECLARE_METATYPE(std::function<QDialog *()>)
