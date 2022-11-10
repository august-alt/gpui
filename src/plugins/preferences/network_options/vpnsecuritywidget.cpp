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

#include "vpnsecuritywidget.h"
#include "ui_vpnsecuritywidget.h"

#include "vpnitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace  preferences
{

VpnSecurityWidget::VpnSecurityWidget(QWidget *parent, VpnItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::VpnSecurityWidget())
{
    ui->setupUi(this);
}

VpnSecurityWidget::~VpnSecurityWidget()
{
    delete ui;
}

void VpnSecurityWidget::setItem(ModelView::SessionItem* item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->passwordCheckBox, VpnItem::propertyToInt(VpnItem::SECURE_PASSWORD));
    mapper->addMapping(ui->encryptionCheckBox, VpnItem::propertyToInt(VpnItem::SECURE_DATA));
    mapper->addMapping(ui->eapRadioButton, VpnItem::propertyToInt(VpnItem::EAP));
    mapper->addMapping(ui->papCheckBox, VpnItem::propertyToInt(VpnItem::PAP));
    mapper->addMapping(ui->spapCheckBox, VpnItem::propertyToInt(VpnItem::SPAP));
    mapper->addMapping(ui->spapCheckBox, VpnItem::propertyToInt(VpnItem::SPAP));
    mapper->addMapping(ui->chapCheckBox, VpnItem::propertyToInt(VpnItem::CHAP));
    mapper->addMapping(ui->mschapCheckBox, VpnItem::propertyToInt(VpnItem::MS_CHAP));
    mapper->addMapping(ui->oldMschapCheckBox, VpnItem::propertyToInt(VpnItem::OLD_MS_CHAP));
    mapper->addMapping(ui->mschap2CheckBox, VpnItem::propertyToInt(VpnItem::MS_CHAP_V2));
    mapper->addMapping(ui->useLogonCheckBox, VpnItem::propertyToInt(VpnItem::USE_LOGON));
    mapper->addMapping(ui->dataComboBox, VpnItem::propertyToInt(VpnItem::ENCRYPTION_TYPE));
    mapper->addMapping(ui->typicalRadioButton, VpnItem::propertyToInt(VpnItem::TYPICAL));
    mapper->addMapping(ui->advancedRadioButton, VpnItem::propertyToInt(VpnItem::ADVANCED));
    mapper->addMapping(ui->otherRadioButton, VpnItem::propertyToInt(VpnItem::USE_OTHER));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

QString VpnSecurityWidget::name() const
{
    return "Security";
}

}
