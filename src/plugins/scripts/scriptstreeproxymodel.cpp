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

    if (role == Qt::UserRole + 14)
    {
        auto contentWidget = new ScriptsContentWidget(nullptr, sessionModel);
        connect(contentWidget, &ScriptsContentWidget::onAccepted, [&]() { snapIn->onDataSave(); });

        auto sessionItem = this->viewModel->sessionItemFromIndex(proxyIndex);

        if (sessionItem->displayName().compare("Scripts") == 0)
        {
            auto nameSpace = sessionItem->property<std::string>("NAMESPACE");
            if (nameSpace.compare("Machine") == 0)
            {
                contentWidget->setItem(machineDataModel->rootItem());
            }
            else
            {
                contentWidget->setItem(userDataModel->rootItem());
            }
        }
        else
        {
            contentWidget->setItem(sessionItem);
        }

        return QVariant::fromValue(contentWidget);
    }

    return QIdentityProxyModel::data(proxyIndex, role);
}

void ScriptsTreeProxyModel::setTreeModel(ModelStruct model)
{
    this->sessionModel     = model.treeModel;
    this->viewModel        = model.viewModel;
    this->machineDataModel = model.machineDataModel;
    this->userDataModel    = model.userDataModel;
    this->snapIn           = model.snapIn;
}

void ScriptsTreeProxyModel::setMachineModel(ModelView::SessionModel *machineDataModelProxy)
{
    this->machineDataModel = machineDataModelProxy;
}

void ScriptsTreeProxyModel::setUserModel(ModelView::SessionModel *userDataModelProxy)
{
    this->userDataModel = userDataModelProxy;
}

} // namespace scripts_plugin

Q_DECLARE_METATYPE(std::shared_ptr<void(QWidget *, QItemSelectionModel *, const QModelIndex &)>)
