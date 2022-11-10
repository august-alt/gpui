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

#include "localprinterwidget.h"
#include "ui_localprinterwidget.h"

#include "localprinteritem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace preferences
{
LocalPrinterWidget::LocalPrinterWidget(QWidget *parent, LocalPrinterItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::LocalPrinterWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

LocalPrinterWidget::~LocalPrinterWidget()
{
    delete ui;
}

void LocalPrinterWidget::setItem(ModelView::SessionItem *item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, LocalPrinterItem::propertyToInt(LocalPrinterItem::ACTION), "currentIndex");
    mapper->addMapping(ui->nameLineEdit, LocalPrinterItem::propertyToInt(LocalPrinterItem::NAME));
    mapper->addMapping(ui->portComboBox, LocalPrinterItem::propertyToInt(LocalPrinterItem::PORT));
    mapper->addMapping(ui->pathLineEdit, LocalPrinterItem::propertyToInt(LocalPrinterItem::PATH));
    mapper->addMapping(ui->defaultCheckBox, LocalPrinterItem::propertyToInt(LocalPrinterItem::DEFAULT));
    mapper->addMapping(ui->locationLineEdit, LocalPrinterItem::propertyToInt(LocalPrinterItem::LOCATION));
    mapper->addMapping(ui->commentLineEdit, LocalPrinterItem::propertyToInt(LocalPrinterItem::COMMENT));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

void LocalPrinterWidget::on_actionComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
}

QString LocalPrinterWidget::name() const
{
    return "General";
}

} // namespace preferences
