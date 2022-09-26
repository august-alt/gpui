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

#ifndef GPUI_POWER_OPTIONS_WIDGET_H
#define GPUI_POWER_OPTIONS_WIDGET_H

#include "common/basepreferencewidget.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class PowerOptionsWidget; }
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

class PowerOptionsItem;

//! Folder item representation for editor.

class PowerOptionsWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit PowerOptionsWidget(QWidget* parent = nullptr, PowerOptionsItem* item = nullptr);
    ~PowerOptionsWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    QString name() const override;

private:
    PowerOptionsWidget(const PowerOptionsWidget&)            = delete;   // copy ctor
    PowerOptionsWidget(PowerOptionsWidget&&)                 = delete;   // move ctor
    PowerOptionsWidget& operator=(const PowerOptionsWidget&) = delete;   // copy assignment
    PowerOptionsWidget& operator=(PowerOptionsWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    PowerOptionsItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::PowerOptionsWidget *ui {nullptr};
};

}

#endif//GPUI_POWER_OPTIONS_WIDGET_H
