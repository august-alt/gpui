/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
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

#include "contentwidget.h"

#include "ui_contentwidget.h"

namespace gpui {

ContentWidget::ContentWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContentWidget())
{
    ui->setupUi(this);

    connect(ui->contentListView, &QListView::clicked, this, &ContentWidget::onListItemClicked);
    connect(this, &ContentWidget::modelItemSelected, this, &ContentWidget::onListItemClicked);
}

ContentWidget::~ContentWidget()
{
    delete ui;
}

void ContentWidget::setModel(QStandardItemModel *model)
{
    ui->contentListView->setModel(model);
}

void ContentWidget::setSelectionModel(QItemSelectionModel *selectionModel)
{
    ui->contentListView->setSelectionModel(selectionModel);
}

void ContentWidget::onListItemClicked(const QModelIndex &index)
{
    const QStandardItemModel* model = dynamic_cast<const QStandardItemModel*>(index.model());

    if (model)
    {
        ui->contentListView->setRootIndex(index);
        ui->descriptionTextEdit->setText(model->itemFromIndex(index)->data(Qt::UserRole + 2).value<QString>());
    }
}

}
