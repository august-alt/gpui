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

#ifndef GPUI_COMMONWIDGET_H
#define GPUI_COMMONWIDGET_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class CommonWidget; }
QT_END_NAMESPACE

namespace gpui
{
    class CommonWidgetPrivate;

    class CommonWidget : public QWidget
    {
        Q_OBJECT

    public:
        CommonWidget(QWidget *parent = 0);
        ~CommonWidget();

        void setupModels(QStandardItemModel* model, QItemSelectionModel* selectionModel);

    private:
        Ui::CommonWidget *ui;

        CommonWidgetPrivate *d;

    private:
        CommonWidget(const CommonWidget&)            = delete;   // copy ctor
        CommonWidget(CommonWidget&&)                 = delete;   // move ctor
        CommonWidget& operator=(const CommonWidget&) = delete;   // copy assignment
        CommonWidget& operator=(CommonWidget&&)      = delete;   // move assignment
    };
}

#endif // GPUI_COMMONWIDGET_H
