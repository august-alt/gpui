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

#include "preferencestreeproxymodel.h"

#include "../../../src/plugins/administrative_templates/bundle/itemtype.h"
#include "../../../src/plugins/administrative_templates/bundle/policyroles.h"

#include "common/preferencesmodel.h"
#include "common/tabledetailswidget.h"

#include "common/preferencecategoryitem.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/viewmodel.h>

#include "preferencessnapinprivate.h"

#include <QIcon>
#include <QUuid>

namespace preferences
{
class PreferencesTreeProxyModelPrivate
{
public:
    std::map<std::string, std::unique_ptr<PreferencesModel>> *machineModels = nullptr;
    std::map<std::string, std::unique_ptr<PreferencesModel>> *userModels    = nullptr;

    gpui::PreferencesSnapInPrivate *snapIn = nullptr;
};

PreferencesTreeProxyModel::PreferencesTreeProxyModel()
    : QIdentityProxyModel()
    , d(new PreferencesTreeProxyModelPrivate())
{}

PreferencesTreeProxyModel::~PreferencesTreeProxyModel()
{
    delete d;
}

QVariant PreferencesTreeProxyModel::data(const QModelIndex &proxyIndex, int role) const
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
        auto viewModel = static_cast<const ModelView::ViewModel *>(proxyIndex.model());
        auto item      = viewModel->sessionItemFromIndex(proxyIndex);
        return QVariant::fromValue(item->property<QUuid>("NODE_ID"));
    }

    if (role == Qt::UserRole + 13)
    {
        auto viewModel = static_cast<const ModelView::ViewModel *>(proxyIndex.model());
        return QVariant::fromValue(viewModel->sessionItemFromIndex(proxyIndex)->property<QUuid>("PARENT_ID"));
    }

    if (role == model::bundle::EXPLAIN_TEXT)
    {
        auto viewModel = static_cast<const ModelView::ViewModel *>(proxyIndex.model());
        auto qtText    = QString::fromStdString(
            viewModel->sessionItemFromIndex(proxyIndex)->property<std::string>("HELP_MSG"));
        return QVariant::fromValue(qtText);
    }

    if (role == model::bundle::POLICY_WIDGET)
    {
        std::function<QWidget *()> widgetCreator = [=]() {
            auto contentWidget = new TableDetailsWidget();

            auto viewModel = static_cast<const ModelView::ViewModel *>(proxyIndex.model());
            auto item      = viewModel->sessionItemFromIndex(proxyIndex);

            if (item->modelType().compare("PreferenceCategoryItem") == 0)
            {
                auto types = item->property<std::map<std::string, QString>>(PreferenceCategoryItem::TYPE);
                if (types.size() > 0)
                {
                    auto modelType = d->machineModels->find(types.begin()->first);
                    if (modelType != d->machineModels->end())
                    {
                        contentWidget->onItemTypeChange(types);
                        contentWidget->setModel(modelType->second.get());
                        QObject::connect(contentWidget,
                                         &TableDetailsWidget::okPressed,
                                         d->snapIn,
                                         &gpui::PreferencesSnapInPrivate::onDataSave);
                    }
                }
            }
            return contentWidget;
        };

        return QVariant::fromValue(widgetCreator);
    }

    if (role == model::bundle::ITEM_TYPE)
    {
        auto viewModel = static_cast<const ModelView::ViewModel *>(proxyIndex.model());
        auto modelType = viewModel->sessionItemFromIndex(proxyIndex)->modelType();
        if (modelType.compare("PreferenceCategoryItem") == 0)
        {
            return QVariant(static_cast<uint>(model::bundle::ITEM_TYPE_POLICY));
        }

        return QVariant(static_cast<uint>(model::bundle::ITEM_TYPE_CATEGORY));
    }

    return QIdentityProxyModel::data(proxyIndex, role);
}

void PreferencesTreeProxyModel::setPreferencesModels(
    std::map<std::string, std::unique_ptr<PreferencesModel>> *machineModels,
    std::map<std::string, std::unique_ptr<PreferencesModel>> *userModels)
{
    d->machineModels = machineModels;
    d->userModels    = userModels;
}

void PreferencesTreeProxyModel::setSnapIn(gpui::PreferencesSnapInPrivate *snapIn)
{
    d->snapIn = snapIn;
}

} // namespace preferences

Q_DECLARE_METATYPE(std::function<QWidget *()>)
