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

#ifndef GPUI_OPEN_WITH_WIDGET_H
#define GPUI_OPEN_WITH_WIDGET_H

#include "common/basepreferencewidget.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class OpenWithWidget; }
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

class OpenWithItem;
class OpenWithItemController;

//! Folder item representation for editor.

class OpenWithWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit OpenWithWidget(QWidget* parent = nullptr, OpenWithItem* item = nullptr);
    ~OpenWithWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    QString name() const override;

private slots:
    void on_actionComboBox_currentIndexChanged(int index);

private:
    OpenWithWidget(const OpenWithWidget&)            = delete;   // copy ctor
    OpenWithWidget(OpenWithWidget&&)                 = delete;   // move ctor
    OpenWithWidget& operator=(const OpenWithWidget&) = delete;   // copy assignment
    OpenWithWidget& operator=(OpenWithWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    OpenWithItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::OpenWithWidget *ui {nullptr};
};

}

#endif//GPUI_OPEN_WITH_WIDGET_H
