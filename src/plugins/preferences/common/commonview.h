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

#ifndef GPUI_COMMONVIEW_H
#define GPUI_COMMONVIEW_H

#include "basepreferencewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CommonView; }
QT_END_NAMESPACE

namespace ModelView
{
    class ViewModel;
    class ViewModelDelegate;
    class SessionItem;
}

namespace preferences
{

class CommonView : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit CommonView(QWidget* parent = nullptr);
    ~CommonView() override;

    void setItem(ModelView::SessionItem *item) override;

    QString name() const override;

private:
    CommonView(const CommonView&)            = delete;   // copy ctor
    CommonView(CommonView&&)                 = delete;   // move ctor
    CommonView& operator=(const CommonView&) = delete;   // copy assignment
    CommonView& operator=(CommonView&&)      = delete;   // move assignment

private:
    Ui::CommonView *ui {nullptr};

private:
    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;
};

}

#endif//GPUI_COMMONVIEW_H
