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

#include "groupmemberitem.h"
#include "groupmemberscontaineritem.h"
#include "groupmemberdialog.h"

#include <mvvm/model/sessionmodel.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/factories/viewmodelfactory.h>

#include <QDebug>

namespace preferences
{

void GroupMembersWidget::on_addPushButton_clicked()
{
    if (!m_item)
    {
        qWarning() << "Invalid m_item";
    }

    auto item = m_item->model()->insertItem<GroupMemberItem>(m_item);
    if (item)
    {
        auto dialog = GroupMemberDialog(this, item);
        dialog.exec();
    }
}

void GroupMembersWidget::on_removePushButton_clicked()
{
    if (m_selectedItem)
    {
        auto parent = m_selectedItem->parent();
        m_selectedItem->model()->removeItem(parent->parent(), parent->tagRow());
    }
    else
    {
        qWarning() << "GroupMembersWidget::on_removePushButton_clicked" << "No item selected.";
    }
}

void GroupMembersWidget::on_changePushButton_clicked()
{
    if (m_selectedItem)
    {
        auto dialog = GroupMemberDialog(this, m_selectedItem->parent());
        dialog.exec();
    }
    else
    {
        qWarning() << "GroupMembersWidget::on_changePushButton_clicked" << "No item selected.";
    }
}

}
