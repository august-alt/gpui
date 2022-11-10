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

#include "vpnoptionswidget.h"
#include "ui_vpnoptionswidget.h"

#include "vpnitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace  preferences
{

VpnOptionsWidget::VpnOptionsWidget(QWidget *parent, VpnItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::VpnOptionsWidget())
{
    ui->setupUi(this);
}

VpnOptionsWidget::~VpnOptionsWidget()
{
    delete ui;
}

void VpnOptionsWidget::setItem(ModelView::SessionItem* item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->displayProgressCheckBox, VpnItem::propertyToInt(VpnItem::SHOW_PROGRESS));
    mapper->addMapping(ui->promptCheckBox, VpnItem::propertyToInt(VpnItem::SHOW_PASSWORD));
    mapper->addMapping(ui->includeCheckBox, VpnItem::propertyToInt(VpnItem::SHOW_DOMAIN));
    mapper->addMapping(ui->redialAttemptsSpinBox, VpnItem::propertyToInt(VpnItem::REDIAL_COUNT));
    mapper->addMapping(ui->timeComboBox, VpnItem::propertyToInt(VpnItem::REDIAL_PAUSE));
    mapper->addMapping(ui->idleTimeComboBox, VpnItem::propertyToInt(VpnItem::IDLE_DISCONNECT));
    mapper->addMapping(ui->redailCheckBox, VpnItem::propertyToInt(VpnItem::RECONNECT));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

QString VpnOptionsWidget::name() const
{
    return "Options";
}

}
