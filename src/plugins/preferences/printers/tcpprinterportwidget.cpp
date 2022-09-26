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

#include "tcpprinterportwidget.h"
#include "ui_tcpprinterportwidget.h"

#include "tcpprinteritem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace  preferences
{

TcpPrinterPortWidget::TcpPrinterPortWidget(QWidget *parent, TcpPrinterItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::TcpPrinterPortWidget())
{
    ui->setupUi(this);
}

TcpPrinterPortWidget::~TcpPrinterPortWidget()
{
    delete ui;
}

void TcpPrinterPortWidget::setItem(ModelView::SessionItem* item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->portSpinBox, TcpPrinterItem::propertyToInt(TcpPrinterItem::PORT_NUMBER));
    mapper->addMapping(ui->queueLineEdit, TcpPrinterItem::propertyToInt(TcpPrinterItem::LPR_QUEUE));
    mapper->addMapping(ui->lprCheckBox, TcpPrinterItem::propertyToInt(TcpPrinterItem::DOUBLE_SPOOL));
    mapper->addMapping(ui->tcpRadioButton, TcpPrinterItem::propertyToInt(TcpPrinterItem::TCP_PROTOCOL));
    mapper->addMapping(ui->lprRadioButton, TcpPrinterItem::propertyToInt(TcpPrinterItem::LPR_PROTOCOL));
    mapper->addMapping(ui->snmpCheckBox, TcpPrinterItem::propertyToInt(TcpPrinterItem::SNMP_ENABLED));
    mapper->addMapping(ui->communityLineEdit, TcpPrinterItem::propertyToInt(TcpPrinterItem::SNMP_COMMUNITY));
    mapper->addMapping(ui->snmpSpinBox, TcpPrinterItem::propertyToInt(TcpPrinterItem::SNMP_DEVINDEX));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

QString TcpPrinterPortWidget::name() const
{
    return "Port";
}

}
