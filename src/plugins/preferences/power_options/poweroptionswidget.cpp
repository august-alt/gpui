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

#include "poweroptionswidget.h"
#include "ui_poweroptionswidget.h"

#include "poweroptionsitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace  preferences
{

PowerOptionsWidget::PowerOptionsWidget(QWidget *parent, PowerOptionsItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::PowerOptionsWidget())
{
    ui->setupUi(this);
}

PowerOptionsWidget::~PowerOptionsWidget()
{
    delete ui;
}

void PowerOptionsWidget::setItem(ModelView::SessionItem* item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->showIconCheckBox, PowerOptionsItem::propertyToInt(PowerOptionsItem::SHOW_ICON));
    mapper->addMapping(ui->promptPasswordCheckBox, PowerOptionsItem::propertyToInt(PowerOptionsItem::PROMPT_PASSWORD));
    mapper->addMapping(ui->enableHibernationCheckBox, PowerOptionsItem::propertyToInt(PowerOptionsItem::ENABLE_HIBERNATION));
    mapper->addMapping(ui->closeLidComboBox, PowerOptionsItem::propertyToInt(PowerOptionsItem::CLOSE_LID));
    mapper->addMapping(ui->pressPowerBtnComboBox, PowerOptionsItem::propertyToInt(PowerOptionsItem::PRESS_POWER_BUTTON));
    mapper->addMapping(ui->pressSleepBtnComboBox, PowerOptionsItem::propertyToInt(PowerOptionsItem::PRESS_SLEEP_BUTTON));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

QString PowerOptionsWidget::name() const
{
    return "Power Options";
}

}
