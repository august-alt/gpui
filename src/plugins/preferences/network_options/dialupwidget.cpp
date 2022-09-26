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

#include "dialupwidget.h"
#include "ui_dialupwidget.h"

#include "dialupitem.h"

#include "common/commonutils.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace  preferences
{

DialUpWidget::DialUpWidget(QWidget *parent, DialUpItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::DialUpWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

DialUpWidget::~DialUpWidget()
{
    delete ui;
}

void DialUpWidget::setItem(ModelView::SessionItem* item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, DialUpItem::propertyToInt(DialUpItem::ACTION));
    mapper->addMapping(ui->connectionLineEdit, DialUpItem::propertyToInt(DialUpItem::NAME));
    mapper->addMapping(ui->phoneLineEdit, DialUpItem::propertyToInt(DialUpItem::PHONE_NUMBER));
    mapper->addMapping(ui->userRadioButton, DialUpItem::propertyToInt(DialUpItem::USER));
    mapper->addMapping(ui->systemRadioButton, DialUpItem::propertyToInt(DialUpItem::SYSTEM));

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

bool DialUpWidget::validate()
{
    if (!CommonUtils::validateLineEdit(ui->connectionLineEdit, tr("Please input connection name value.")))
    {
        return false;
    }

    if (!CommonUtils::validateLineEdit(ui->phoneLineEdit, tr("Please input phone value.")))
    {
        return false;
    }

    return true;
}

QString DialUpWidget::name() const
{
    return "General";
}

}
