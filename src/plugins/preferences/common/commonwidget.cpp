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

#include "commonwidget.h"
#include "ui_commonwidget.h"

#include "commonroles.h"

namespace gpui
{

class CommonWidgetPrivate
{
public:
    std::unique_ptr<QDataWidgetMapper> mapper;
};

CommonWidget::CommonWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CommonWidget())
    , d(new CommonWidgetPrivate())
{
    ui->setupUi(this);

    d->mapper = std::make_unique<QDataWidgetMapper>(this);
}

CommonWidget::~CommonWidget()
{
    delete ui;
}

void CommonWidget::setupModels(QStandardItemModel *model, QItemSelectionModel *selectionModel)
{
    d->mapper->setModel(model);

    connect(selectionModel, &QItemSelectionModel::currentRowChanged, d->mapper.get(),
            &QDataWidgetMapper::setCurrentModelIndex);

    d->mapper->addMapping(ui->stopOnErrorCheckBox, CommonRoles::BYPASS_ERRORS, "checked");
    d->mapper->addMapping(ui->descriptionTextEdit, CommonRoles::DESC);
    d->mapper->addMapping(ui->removeThisCheckBox, CommonRoles::REMOVE_POLICY, "checked");
    d->mapper->addMapping(ui->userContextCheckBox, CommonRoles::USER_CONTEXT, "checked");

    d->mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

}
