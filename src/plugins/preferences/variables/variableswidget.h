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

#ifndef GPUI_VARIABLES_WIDGET_H
#define GPUI_VARIABLES_WIDGET_H

#include <QtWidgets>

#include "common/basepreferencewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class VariablesWidget; }
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

class VariablesItem;
class VariablesItemController;

//! Folder item representation for editor.

class VariablesWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit VariablesWidget(QWidget* parent = nullptr, VariablesItem* item = nullptr);
    ~VariablesWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    bool validate() override;

    QString name() const override;

private slots:
    void on_actionComboBox_currentIndexChanged(int index);
    void on_pathCheckBox_stateChanged(int state);
    void on_systemVariableRadio_clicked();
    void on_userVariableRadio_clicked();

    void setDetailsText(int index);

private:
    void setDefaultValues();

private:
    VariablesWidget(const VariablesWidget&)            = delete;   // copy ctor
    VariablesWidget(VariablesWidget&&)                 = delete;   // move ctor
    VariablesWidget& operator=(const VariablesWidget&) = delete;   // copy assignment
    VariablesWidget& operator=(VariablesWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    VariablesItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::VariablesWidget *ui {nullptr};
};

}

#endif//GPUI_VARIABLES_WIDGET_H
