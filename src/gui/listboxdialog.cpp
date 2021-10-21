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

#include "listboxdialog.h"

#include "ui_listboxdialog.h"

namespace gpui
{

ListBoxDialog::ListBoxDialog(const QString &dialogTitle)
    : QDialog()
    , ui(new Ui::ListBoxDialog())
{
    ui->setupUi(this);    

    setupListBoxWidget();

    this->setWindowTitle(tr("List Dialog"));

    ui->nameLabel->setText(dialogTitle);

    connect(ui->newButton, &QPushButton::clicked, this, &ListBoxDialog::onNew);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ListBoxDialog::onDelete);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
            [&](){
                QStringList items;
                for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
                {
                    QTableWidgetItem* item = ui->tableWidget->item(i, 1);
                    if (item && item->data(Qt::DisplayRole).isValid())
                    {
                        items.push_back(item->text());
                    }
                }
                itemsEditingFinished(items);
            });
}

ListBoxDialog::~ListBoxDialog()
{
    delete ui;
}

void ListBoxDialog::setItems(const QStringList &items)
{
    int index = 0;
    ui->tableWidget->setRowCount(items.size());
    for (const auto& itemString : items)
    {
        ui->tableWidget->setItem(index, 1, new QTableWidgetItem(itemString));
        index++;
    }
}

void ListBoxDialog::clearItems()
{
    ui->tableWidget->clearContents();
}

void ListBoxDialog::setupListBoxWidget()
{
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(1);
    QStringList header { "#", tr("Name") };
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->hideColumn(0);
}

void ListBoxDialog::onNew()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
}

void ListBoxDialog::onDelete()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

}
