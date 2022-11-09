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

#include "registrywidget.h"
#include "ui_registrywidget.h"

#include "common/commonutils.h"
#include "registryitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace preferences
{
RegistryWidget::RegistryWidget(QWidget *parent, RegistryItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::RegistryWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());

    setInitialState();
}

RegistryWidget::~RegistryWidget()
{
    delete ui;
}

void RegistryWidget::setItem(ModelView::SessionItem *item)
{
    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->actionComboBox, 0, "currentIndex");
    mapper->addMapping(ui->hiveComboBox, 1);
    mapper->addMapping(ui->keyPathLineEdit, 2);
    mapper->addMapping(ui->valueNameLineEdit, 3);
    mapper->addMapping(ui->valueTypeComboBox, 4);
    mapper->addMapping(ui->valueDataLineEdit, 5);
    mapper->addMapping(ui->defaultValueNameCheckBox, 6);

    mapper->setCurrentModelIndex(view_model->index(0, 1));

    setInitialState();
}

bool RegistryWidget::validate()
{
    if (!CommonUtils::validateLineEdit(ui->keyPathLineEdit, tr("Please input key path value.")))
    {
        return false;
    }

    if (!CommonUtils::validateLineEdit(ui->valueNameLineEdit, tr("Please input name value.")))
    {
        return false;
    }

    if (!CommonUtils::validateLineEdit(ui->valueDataLineEdit, tr("Please input data value.")))
    {
        return false;
    }

    return true;
}

QString RegistryWidget::name() const
{
    return tr("General");
}

} // namespace preferences
