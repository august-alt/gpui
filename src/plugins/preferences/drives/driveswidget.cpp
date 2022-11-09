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

#include "driveswidget.h"
#include "ui_driveswidget.h"

#include "common/commonutils.h"
#include "drivesitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace preferences
{
DrivesWidget::DrivesWidget(QWidget *parent, DrivesItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::DrivesWidget())
{
    ui->setupUi(this);

    ui->thisButtonGroup->setId(ui->noChangeThisRadioButton, 0);
    ui->thisButtonGroup->setId(ui->hideThisRadioButton, 1);
    ui->thisButtonGroup->setId(ui->showThisRadioButton, 2);

    ui->allButtonGroup->setId(ui->noChangeAllRadioButton, 0);
    ui->allButtonGroup->setId(ui->hideAllRadioButton, 1);
    ui->allButtonGroup->setId(ui->showAllRadioButton, 2);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

DrivesWidget::~DrivesWidget()
{
    delete ui;
}

void DrivesWidget::setItem(ModelView::SessionItem *item)
{
    m_item = dynamic_cast<DrivesItem *>(item);

    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, 0, "currentIndex");
    mapper->addMapping(ui->pathLineEdit, 1);
    mapper->addMapping(ui->reconnectCheckBox, 2);
    mapper->addMapping(ui->labelLineEdit, 3);
    mapper->addMapping(ui->driveComboBox, 4);
    mapper->addMapping(ui->driveRadioButton, 7);
    mapper->addMapping(ui->firstAvailableRadioButton, 10);

    setThisDriveCheckBox(view_model->index(8, 1).data().toInt());
    setAllDrivesCheckBox(view_model->index(9, 1).data().toInt());

    mapper->setCurrentModelIndex(view_model->index(0, 1));

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

QString DrivesWidget::name() const
{
    return tr("General");
}

bool DrivesWidget::validate()
{
    if (!CommonUtils::validateLineEdit(ui->pathLineEdit, tr("Please enter location.")))
    {
        return false;
    }

    return true;
}

} // namespace preferences
