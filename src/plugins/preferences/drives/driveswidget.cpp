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

#include "driveswidget.h"
#include "ui_driveswidget.h"

#include "driveroles.h"

namespace gpui
{

class DrivesWidgetPrivate
{
public:
    std::unique_ptr<QDataWidgetMapper> mapper;
    QStandardItemModel* model;
    QItemSelectionModel* selectionModel;

    DrivesWidgetPrivate(QStandardItemModel* model, QItemSelectionModel* selectionModel)
        : model(model)
        , selectionModel(selectionModel)
    {

    }
};

DrivesWidget::DrivesWidget(QStandardItemModel& model, QItemSelectionModel& selectionModel, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DrivesWidget())
    , d(new DrivesWidgetPrivate(&model, &selectionModel))
{
    ui->setupUi(this);

    d->mapper = std::make_unique<QDataWidgetMapper>(this);
    d->mapper->setModel(d->model);

    connect(d->selectionModel, &QItemSelectionModel::currentRowChanged, d->mapper.get(),
            &QDataWidgetMapper::setCurrentModelIndex);

    setupModelMapper();
}

DrivesWidget::~DrivesWidget()
{
    delete ui;

    delete d;
}

void DrivesWidget::setupModelMapper()
{
    d->mapper->addMapping(ui->pathLineEdit, DriveRoles::PATH);
    d->mapper->addMapping(ui->reconnectCheckBox, DriveRoles::PERSISTENT);
    d->mapper->addMapping(ui->labelLineEdit, DriveRoles::LABEL);
    d->mapper->addMapping(ui->driveLetterGroupBox, DriveRoles::LETTER);
    d->mapper->addMapping(ui->userNameLineEdit, DriveRoles::USERNAME);

    d->mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

}
