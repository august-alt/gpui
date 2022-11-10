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

#ifndef GPUI_LOCALPRINTER_WIDGET_H
#define GPUI_LOCALPRINTER_WIDGET_H

#include "common/basepreferencewidget.h"

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class LocalPrinterWidget; }
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

class LocalPrinterItem;

//! LocalPrinter representation for editor.

class LocalPrinterWidget : public BasePreferenceWidget
{
public:
    Q_OBJECT

public:
    explicit LocalPrinterWidget(QWidget* parent = nullptr, LocalPrinterItem* item = nullptr);
    ~LocalPrinterWidget() override;

    void setItem(ModelView::SessionItem *item) override;

    QString name() const override;

private slots:
    void on_actionComboBox_currentIndexChanged(int index);

private:
    LocalPrinterWidget(const LocalPrinterWidget&)            = delete;   // copy ctor
    LocalPrinterWidget(LocalPrinterWidget&&)                 = delete;   // move ctor
    LocalPrinterWidget& operator=(const LocalPrinterWidget&) = delete;   // copy assignment
    LocalPrinterWidget& operator=(LocalPrinterWidget&&)      = delete;   // move assignment

private:
    //!< Underlying item of this view.
    LocalPrinterItem* m_item {nullptr};

    std::unique_ptr<ModelView::ViewModel> view_model;
    std::unique_ptr<ModelView::ViewModelDelegate> delegate;

private:
    Ui::LocalPrinterWidget *ui {nullptr};
};

}

#endif//GPUI_LOCALPRINTER_WIDGET_H

