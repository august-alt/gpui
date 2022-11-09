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

#include "sharedprinterwidget.h"
#include "ui_sharedprinterwidget.h"

#include "sharedprinteritem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace preferences
{
SharedPrinterWidget::SharedPrinterWidget(QWidget *parent, SharedPrinterItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::SharedPrinterWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

SharedPrinterWidget::~SharedPrinterWidget()
{
    delete ui;
}

void SharedPrinterWidget::setItem(ModelView::SessionItem *item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, SharedPrinterItem::propertyToInt(SharedPrinterItem::ACTION), "currentIndex");
    mapper->addMapping(ui->sharedPathLineEdit, SharedPrinterItem::propertyToInt(SharedPrinterItem::PATH));
    mapper->addMapping(ui->setPrinterCheckBox, SharedPrinterItem::propertyToInt(SharedPrinterItem::DEFAULT));
    mapper->addMapping(ui->onlyIfCheckBox, SharedPrinterItem::propertyToInt(SharedPrinterItem::SKIP_LOCAL));
    mapper->addMapping(ui->deleteCheckBox, SharedPrinterItem::propertyToInt(SharedPrinterItem::DELETE_ALL));
    mapper->addMapping(ui->localPortComboBox, SharedPrinterItem::propertyToInt(SharedPrinterItem::PORT));
    mapper->addMapping(ui->reconnectCheckBox, SharedPrinterItem::propertyToInt(SharedPrinterItem::PERSISTENT));
    mapper->addMapping(ui->unmapCheckBox, SharedPrinterItem::propertyToInt(SharedPrinterItem::DELETE_MAPS));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

void SharedPrinterWidget::on_actionComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
}

QString SharedPrinterWidget::name() const
{
    return "General";
}

} // namespace preferences
