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

#include "snapindetailswidget.h"
#include "ui_snapindetailswidget.h"

#include <QWidget>

namespace gpui
{
SnapInDetailsWidget::SnapInDetailsWidget(QWidget *parent, ::gpui::ISnapIn *snapIn)
    : SnapInDetailsWidget(parent)
{
    setSnapIn(snapIn);
}

SnapInDetailsWidget::SnapInDetailsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SnapInDetailsWidget())
{
    ui->setupUi(this);
}

SnapInDetailsWidget::~SnapInDetailsWidget()
{
    delete ui;
}

void SnapInDetailsWidget::setSnapIn(const ISnapIn *snapIn)
{
    ui->categoryLineEdit->setText(snapIn->getId().toString());
    ui->copyrightLineEdit->setText(snapIn->getCopyright());
    ui->descriptionPlainTextEdit->setPlainText(snapIn->getHelpText());
    ui->licensePlainTextEdit->setPlainText(snapIn->getLicense());
    ui->nameLineEdit->setText(snapIn->getDisplayName());
    ui->versionLineEdit->setText(snapIn->getVersion().toString());
}

} // namespace gpui
