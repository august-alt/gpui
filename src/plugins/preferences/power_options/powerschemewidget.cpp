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

#include "powerschemewidget.h"
#include "ui_powerschemewidget.h"

#include "powerschemeitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

const int DELETE__MODE = 4;

namespace  preferences
{

PowerSchemeWidget::PowerSchemeWidget(QWidget *parent, PowerSchemeItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::PowerSchemeWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

PowerSchemeWidget::~PowerSchemeWidget()
{
    delete ui;
}

void PowerSchemeWidget::setItem(ModelView::SessionItem* item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::ACTION));
    mapper->addMapping(ui->nameComboBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::NAME));
    mapper->addMapping(ui->defaultCheckBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::DEFAULT));

    mapper->addMapping(ui->monitorAcComboBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::MONITOR_AC));
    mapper->addMapping(ui->standbyAcComboBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::STANDBY_AC));
    mapper->addMapping(ui->hardDiskAcComboBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::HARD_DISK_AC));
    mapper->addMapping(ui->hibernateAcComboBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::HIBERNATE_AC));

    mapper->addMapping(ui->monitorDcComboBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::MONITOR_DC));
    mapper->addMapping(ui->standbyDcComboBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::STANDBY_DC));
    mapper->addMapping(ui->hardDiskDcComboBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::HARD_DISK_DC));
    mapper->addMapping(ui->hibernateDcComboBox, PowerSchemeItem::propertyToInt(PowerSchemeItem::HIBERNATE_DC));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

void PowerSchemeWidget::on_actionComboBox_currentIndexChanged(int index)
{
    bool disable = index == DELETE__MODE;
    ui->settingGroupBox->setDisabled(disable);
    ui->defaultCheckBox->setDisabled(disable);
}

QString PowerSchemeWidget::name() const
{
    return "General";
}

}
