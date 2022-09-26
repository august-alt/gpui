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

#ifndef GPUI_LOCALGROUP_WIDGET_H
#define GPUI_LOCALGROUP_WIDGET_H

#include "common/basepreferencewidget.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class LocalGroupWidget; }
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

class LocalGroupItem;
class LocalGroupItemController;

//! Folder item representation for editor.

class LocalGroupWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit LocalGroupWidget(QWidget* parent = nullptr, LocalGroupItem* item = nullptr);
    ~LocalGroupWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    QString name() const override;

private slots:
    void on_actionComboBox_currentIndexChanged(int index);

private:
    LocalGroupWidget(const LocalGroupWidget&)            = delete;   // copy ctor
    LocalGroupWidget(LocalGroupWidget&&)                 = delete;   // move ctor
    LocalGroupWidget& operator=(const LocalGroupWidget&) = delete;   // copy assignment
    LocalGroupWidget& operator=(LocalGroupWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    LocalGroupItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::LocalGroupWidget *ui {nullptr};
};

}

#endif//GPUI_LOCALGROUP_WIDGET_H
