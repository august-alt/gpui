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

#include "groupmemberdialog.h"
#include "ui_groupmemberdialog.h"

#include "groupmemberitem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <QDataWidgetMapper>

namespace  preferences
{

GroupMemberDialog::GroupMemberDialog(QWidget *parent, ModelView::SessionItem* item)
    : QDialog(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , mapper(nullptr)
    , ui(new Ui::GroupMemberDialog())
{
    ui->setupUi(this);

    if (item)
    {
        setItem(item);
    }
}

GroupMemberDialog::~GroupMemberDialog()
{
    delete ui;
}

void GroupMemberDialog::setItem(ModelView::SessionItem* item)
{
    m_item = item;

    view_model = ModelView::Factory::CreatePropertyFlatViewModel(item->model());
    view_model->setRootSessionItem(item);

    mapper = std::make_unique<QDataWidgetMapper>();

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    mapper->setModel(view_model.get());
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());

    mapper->addMapping(ui->nameLineEdit, 0);
    mapper->addMapping(ui->actionComboBox, 1);

    mapper->setCurrentModelIndex(view_model->index(0, 1));
}

bool GroupMemberDialog::validate()
{
    // TODO: Implement.

    return true;
}

void GroupMemberDialog::submit()
{
    if (mapper && validate())
    {
        mapper->submit();

        emit dataChanged();
    }
}

}
