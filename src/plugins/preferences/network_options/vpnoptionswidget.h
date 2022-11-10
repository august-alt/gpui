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

#ifndef GPUI_VPN_OPTIONS_WIDGET_H
#define GPUI_VPN_OPTIONS_WIDGET_H

#include "common/basepreferencewidget.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class VpnOptionsWidget; }
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

class VpnItem;

//! Vpn options widget.

class VpnOptionsWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit VpnOptionsWidget(QWidget* parent = nullptr, VpnItem* item = nullptr);
    ~VpnOptionsWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    QString name() const override;

private:
    VpnOptionsWidget(const VpnOptionsWidget&)            = delete;   // copy ctor
    VpnOptionsWidget(VpnOptionsWidget&&)                 = delete;   // move ctor
    VpnOptionsWidget& operator=(const VpnOptionsWidget&) = delete;   // copy assignment
    VpnOptionsWidget& operator=(VpnOptionsWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    VpnItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::VpnOptionsWidget *ui {nullptr};
};

}

#endif//GPUI_VPN_OPTIONS_WIDGET_H
