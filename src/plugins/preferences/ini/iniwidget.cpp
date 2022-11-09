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

#include "iniwidget.h"
#include "ui_iniwidget.h"

#include "common/commonutils.h"
#include "iniitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace preferences
{
IniWidget::IniWidget(QWidget *parent, IniItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::IniWidget())
{
    ui->setupUi(this);

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

IniWidget::~IniWidget()
{
    delete ui;
}

void IniWidget::setItem(ModelView::SessionItem *item)
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
    mapper->addMapping(ui->pathLineEdit, 1);
    mapper->addMapping(ui->sectionLineEdit, 2);
    mapper->addMapping(ui->valueLineEdit, 3);
    mapper->addMapping(ui->propertyLineEdit, 4);

    mapper->setCurrentModelIndex(view_model->index(0, 1));

    on_actionComboBox_currentIndexChanged(ui->actionComboBox->currentIndex());
}

bool IniWidget::validate()
{
    if (!CommonUtils::validateLineEdit(ui->pathLineEdit, tr("Please input path value.")))
    {
        return false;
    }

    if (!CommonUtils::validateLineEdit(ui->sectionLineEdit, tr("Please input section value.")))
    {
        return false;
    }

    if (!CommonUtils::validateLineEdit(ui->propertyLineEdit, tr("Please input property value.")))
    {
        return false;
    }

    return true;
}

QString IniWidget::name() const
{
    return tr("General");
}

} // namespace preferences
