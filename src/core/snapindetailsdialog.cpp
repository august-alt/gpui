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

#include "snapindetailsdialog.h"
#include "ui_snapindetailsdialog.h"

#include <QDialog>

namespace gpui
{
SnapInDetailsDialog::SnapInDetailsDialog(QWidget *parent, ::gpui::ISnapIn *snapIn)
    : SnapInDetailsDialog(parent)
{
    setSnapIn(snapIn);
}

SnapInDetailsDialog::SnapInDetailsDialog(QWidget *parent)
    : ISnapInDetailsDialog(parent)
    , ui(new Ui::SnapInDetailsDialog())
{
    ui->setupUi(this);
}

SnapInDetailsDialog::~SnapInDetailsDialog()
{
    delete ui;
}

void SnapInDetailsDialog::setSnapIn(ISnapIn *snapIn)
{
    ui->snapInWidget->setSnapIn(snapIn);
}
} // namespace gpui
