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

#ifndef GPUI_IMMEDIATESETTINGSWIN7_WIDGET_H
#define GPUI_IMMEDIATESETTINGSWIN7_WIDGET_H

#include "common/basepreferencewidget.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class ImmediateSettingsWin7Widget; }
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

class ImmediateSettingsWin7Item;

//! ImmediateSettingsWin7 representation for editor.

class ImmediateSettingsWin7Widget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit ImmediateSettingsWin7Widget(QWidget* parent = nullptr, ImmediateSettingsWin7Item* item = nullptr);
    ~ImmediateSettingsWin7Widget() override;

    void setItem(ModelView::SessionItem *item) override;

private slots:
    void on_actionComboBox_currentIndexChanged(int index);

private:
    ImmediateSettingsWin7Widget(const ImmediateSettingsWin7Widget&)            = delete;   // copy ctor
    ImmediateSettingsWin7Widget(ImmediateSettingsWin7Widget&&)                 = delete;   // move ctor
    ImmediateSettingsWin7Widget& operator=(const ImmediateSettingsWin7Widget&) = delete;   // copy assignment
    ImmediateSettingsWin7Widget& operator=(ImmediateSettingsWin7Widget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    ImmediateSettingsWin7Item* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::ImmediateSettingsWin7Widget *ui {nullptr};
};

}

#endif//GPUI_IMMEDIATESETTINGSWIN7_WIDGET_H

