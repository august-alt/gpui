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

#include "propertieswidget.h"
#include "ui_propertieswidget.h"

#include "common/commonitem.h"

#include "common/preferencesdialog.h"
#include "interfaces/containeriteminterface.h"
#include "shortcuts/shortcutscontaineritem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/viewmodel/propertytableviewmodel.h>
#include <mvvm/viewmodel/viewitem.h>

#include "propertieswidgetdelegate.h"

#include <QStyledItemDelegate>

namespace preferences
{
PropertiesWidget::PropertiesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PropertiesWidget())
    , view_model(nullptr)
    , delegate(new PropertiesWidgetDelegate())
    , mapper(nullptr)
{
    ui->setupUi(this);
}

PropertiesWidget::~PropertiesWidget()
{
    delete ui;
}

void PropertiesWidget::setItem(ModelView::SessionItem *item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->stopProcessingStatusLabel, CommonItem::propertyToInt(CommonItem::BYPASS_ERRORS));
    mapper->addMapping(ui->runInUsersContextStatusLabel, CommonItem::propertyToInt(CommonItem::USER_CONTEXT));
    mapper->addMapping(ui->removeIfNotAppliedStatusLabel, CommonItem::propertyToInt(CommonItem::REMOVE_POLICY));

    mapper->addMapping(ui->plainTextEdit, CommonItem::propertyToInt(CommonItem::DESC));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

} // namespace preferences
