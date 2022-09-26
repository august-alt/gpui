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

#ifndef GPUI_GROUPMEMBER_DIALOG_H
#define GPUI_GROUPMEMBER_DIALOG_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class GroupMemberDialog; }
QT_END_NAMESPACE

class QDataWidgetMapper;

namespace ModelView
{
class ViewModel;
class ViewModelDelegate;
class SessionItem;
}

namespace preferences
{

class GroupMemberItem;

class GroupMemberDialog : public QDialog
{
public:
    Q_OBJECT

public:
    explicit GroupMemberDialog(QWidget* parent = nullptr, ModelView::SessionItem* item = nullptr);
    ~GroupMemberDialog() override;

    void setItem(ModelView::SessionItem *item);

    bool validate();

signals:
    void dataChanged();

public slots:
    void submit();

private:
    GroupMemberDialog(const GroupMemberDialog&)            = delete;   // copy ctor
    GroupMemberDialog(GroupMemberDialog&&)                 = delete;   // move ctor
    GroupMemberDialog& operator=(const GroupMemberDialog&) = delete;   // copy assignment
    GroupMemberDialog& operator=(GroupMemberDialog&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    ModelView::SessionItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;
    std::unique_ptr<QDataWidgetMapper> mapper;

private:
    Ui::GroupMemberDialog *ui {nullptr};
};

}

#endif//GPUI_GROUPMEMBER_DIALOG_H
