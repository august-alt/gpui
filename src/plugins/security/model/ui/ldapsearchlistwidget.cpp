/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#include "ldapsearchlistwidget.h"
#include "ui_ldapsearchlistwidget.h"

#include "browseldapdialog.h"

#include <QDataWidgetMapper>

namespace security
{

class LdapSearchListWidgetPrivate
{
public:
    QString filter{};
    QString dn{};

    LdapSearchListWidgetPrivate(const QString &newFilter, const QString &newDN)
        : filter(newFilter)
        , dn(newDN)
    {}
};

LdapSearchListWidget::LdapSearchListWidget(const QString &filter, const QString &dn, QWidget *parent)
    : QWidget(parent)
    , d(new LdapSearchListWidgetPrivate(filter, dn))
    , ui(new Ui::LdapSearchListWidget())
{
    ui->setupUi(this);
}

LdapSearchListWidget::LdapSearchListWidget(const std::string &filter, const std::string &dn, QWidget *parent)
    : LdapSearchListWidget(QString::fromStdString(filter), QString::fromStdString(dn), parent)
{
}

LdapSearchListWidget::~LdapSearchListWidget()
{
    delete d;
    delete ui;
}

void LdapSearchListWidget::setAddButtonText(const QString &text)
{
    ui->addButton->setText(text);
}

void LdapSearchListWidget::setRemoveButtonText(const QString &text)
{
    ui->removeButton->setText(text);
}

void LdapSearchListWidget::on_addButton_Pressed()
{
    auto browseLdapDialog = new BrowseLdapDialog(this);
    browseLdapDialog->show();
}

void LdapSearchListWidget::on_removeButton_Pressed()
{
    auto selectedItems = ui->listWidget->selectedItems();

    for (const auto& selectedItem : selectedItems)
    {
        ui->listWidget->removeItemWidget(selectedItem);
        delete selectedItem;
    }
}

}