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

#include "groupmemberswidget.h"
#include "ui_groupmemberswidget.h"

#include "groupmemberscontaineritem.h"

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodeldelegate.h>

#include <mvvm/viewmodel/propertyviewmodel.h>
#include <mvvm/viewmodel/propertyflatviewmodel.h>
#include <mvvm/viewmodel/propertytableviewmodel.h>

#include <QDataWidgetMapper>

namespace  preferences
{

GroupMembersWidget::GroupMembersWidget(QWidget *parent, GroupMembersContainerItem *item)
    : BasePreferenceWidget(parent)
    , m_item(item)
    , view_model(nullptr)
    , delegate(std::make_unique<ModelView::ViewModelDelegate>())
    , ui(new Ui::GroupMembersWidget())
{
    ui->setupUi(this);
}

GroupMembersWidget::~GroupMembersWidget()
{
    delete ui;
}

void preferences::GroupMembersWidget::setupConnections()
{
    connect(ui->membersTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            [&](const QItemSelection &selected, const QItemSelection &deselected)
    {
        Q_UNUSED(deselected);

        if (view_model && selected.indexes().size() > 0)
        {
            QModelIndex selectedIndex = selected.indexes().at(0);
            m_selectedItem = view_model->sessionItemFromIndex(selectedIndex);
        }
    });
}

void GroupMembersWidget::setItem(ModelView::SessionItem* item)
{
    m_item = dynamic_cast<GroupMembersContainerItem*>(item);

    view_model = ModelView::Factory::CreatePropertyTableViewModel(item->model());
    view_model->setRootSessionItem(item);

    ui->membersTableView->setModel(view_model.get());
    ui->membersTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setupConnections();
}

QString GroupMembersWidget::name() const
{
    return "Group Members";
}

}

