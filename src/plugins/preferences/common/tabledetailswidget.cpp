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

#include "tabledetailswidget.h"
#include "ui_tabledetailswidget.h"

#include "common/commonitem.h"

#include "common/preferencesdialog.h"
#include "interfaces/containeriteminterface.h"
#include "shortcuts/shortcutscontaineritem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/viewmodel/propertytableviewmodel.h>
#include <mvvm/viewmodel/viewitem.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDebug>

namespace preferences
{
TableDetailsWidget::TableDetailsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TableDetailsWidget())
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , mapper(nullptr)
    , itemTypes()
    , modelType(0)
{
    ui->setupUi(this);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
}

TableDetailsWidget::~TableDetailsWidget()
{
    delete ui;
}

void TableDetailsWidget::setModel(ModelView::SessionModel *model)
{
    view_model = std::make_unique<ModelView::PropertyTableViewModel>(model);

    ui->treeView->setModel(view_model.get());
    ui->treeView->setItemDelegate(delegate.get());
    ui->treeView->header()->setSectionResizeMode(QHeaderView::Stretch);

    setupConnections();

    ui->treeView->setCurrentIndex(view_model->index(0, 0));
}

void TableDetailsWidget::setModelType(ModelType newModelType)
{
    modelType = newModelType;
}

void TableDetailsWidget::onItemTypeChange(const std::map<std::string, QString> &newItemTypes)
{
    itemTypes = newItemTypes;
}

void TableDetailsWidget::on_treeView_customContextMenuRequested(const QPoint &point)
{
    if (!view_model)
    {
        return;
    }

    auto treeView = qobject_cast<QTreeView *>(sender());

    QModelIndex index = treeView->indexAt(point);

    auto view_item = view_model->itemFromIndex(index);

    QMenu menu;

    auto newMenuItem = menu.addMenu(tr("New"));

    if (view_item)
    {
        auto item   = view_item->item()->parent();
        auto tagrow = item->tagRow();

        // removing item under the mouse
        auto removeItemAction = menu.addAction(tr("Remove item"));
        auto remove_item      = [=]() {
            view_model->sessionModel()->removeItem(item->parent(), tagrow);
            ui->treeView->selectionModel()->select(QItemSelection(), QItemSelectionModel::ClearAndSelect);
            emit okPressed();
        };
        connect(removeItemAction, &QAction::triggered, remove_item);
    }

    menu.addSeparator();
    auto helpAction = menu.addAction(tr("Help"));

    auto show_help = [=]() {
        const QUrl manual_url = QUrl("https://www.altlinux.org/"
                                     "%D0%93%D1%80%D1%83%D0%BF%D0%BF%D0%BE%D0%B2%D1%8B%D0%B5_%D0%BF%D0%BE%D0%BB%D0%B8%"
                                     "D1%82%D0%B8%D0%BA%D0%B8/GPUI");
        QDesktopServices::openUrl(manual_url);
    };
    connect(helpAction, &QAction::triggered, show_help);

    for (const auto &itemType : itemTypes)
    {
        auto addItemAction = newMenuItem->addAction(itemType.second);
        auto add_item      = [&]() {
            auto newItem = view_model->sessionModel()->insertNewItem(itemType.first,
                                                                     view_model->sessionModel()->rootItem());
            if (auto containerItemInterface = dynamic_cast<ContainerItemInterface *>(newItem))
            {
                containerItemInterface->setupListeners();
            }

            newItem->children()[newItem->childrenCount() - 2]->setProperty<bool>(CommonItem::propertyToString(
                                                                                     CommonItem::USER_CONTEXT),
                                                                                 modelType == ModelType::USER);

            auto preferencesDialog = new PreferencesDialog(newItem, this);
            connect(preferencesDialog, &QDialog::rejected, [&]() {
                view_model->sessionModel()->removeItem(newItem->parent(), newItem->tagRow());
            });

            connect(preferencesDialog, &QDialog::accepted, [&]() {
                emit okPressed();
                ui->treeView->selectionModel()->select(view_model->index(view_model->rowCount() - 1, 0),
                                                       QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            });
            preferencesDialog->exec();
        };
        connect(addItemAction, &QAction::triggered, add_item);
    }

    menu.exec(treeView->mapToGlobal(point));
}

void TableDetailsWidget::on_treeView_doubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        auto item              = view_model->sessionItemFromIndex(index);
        auto preferencesDialog = new PreferencesDialog(item->parent(), this);
        connect(preferencesDialog, &QDialog::accepted, [&]() { emit okPressed(); });
        preferencesDialog->exec();
    }
}

void TableDetailsWidget::setupConnections()
{
    connect(ui->treeView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            [&](const QItemSelection &selected, const QItemSelection &deselected) {
                Q_UNUSED(deselected);
                if (selected.isEmpty() || selected.first().indexes().isEmpty())
                {
                    ui->propertiesWidget->setDescriptionVisibility(false);
                    return;
                }

                auto indexes = selected.indexes();
                if (!indexes.empty())
                {
                    auto item = view_model->sessionItemFromIndex(indexes.at(0));
                    ui->propertiesWidget->setItem(item->parent()->children()[item->parent()->children().size() - 2]);
                    ui->propertiesWidget->setDescriptionVisibility(true);
                }
                else
                {
                    ui->propertiesWidget->setDescriptionVisibility(false);
                }
            });
}

} // namespace preferences
