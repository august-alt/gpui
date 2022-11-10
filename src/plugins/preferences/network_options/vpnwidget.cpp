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

#include "vpnwidget.h"
#include "ui_vpnwidget.h"

#include "vpnitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace  preferences
{

VpnWidget::VpnWidget(QWidget *parent, VpnItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::VpnWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

VpnWidget::~VpnWidget()
{
    delete ui;
}

void VpnWidget::setItem(ModelView::SessionItem* item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, VpnItem::propertyToInt(VpnItem::ACTION));
    mapper->addMapping(ui->userConnectionRadioButton, VpnItem::propertyToInt(VpnItem::USER));
    mapper->addMapping(ui->connectionNameLineEdit, VpnItem::propertyToInt(VpnItem::NAME)); 
    mapper->addMapping(ui->ipaddressLineEdit, VpnItem::propertyToInt(VpnItem::IP_ADDRESS));
    mapper->addMapping(ui->ipaddressLabel, VpnItem::propertyToInt(VpnItem::IP_ADDRESS));
    mapper->addMapping(ui->useDnsCheckBox, VpnItem::propertyToInt(VpnItem::USE_DNS));
    mapper->addMapping(ui->dialLineEdit, VpnItem::propertyToInt(VpnItem::DIAL_FIRST));
    mapper->addMapping(ui->showIconCheckBox, VpnItem::propertyToInt(VpnItem::TRAY_ICON));

    mapper->setCurrentModelIndex(view_model->index(0, 1));

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

QString VpnWidget::name() const
{
    return "General";
}

}
