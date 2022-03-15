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

#ifndef GPUI_FOLDERSWIDGET_H
#define GPUI_FOLDERSWIDGET_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class FoldersWidget; }
QT_END_NAMESPACE

namespace gpui
{
    class FoldersWidget : public QWidget
    {
        Q_OBJECT

    public:
        FoldersWidget(QStandardItemModel &model, QItemSelectionModel &selectionModel, QWidget *parent = 0);
        ~FoldersWidget();

    private:
        FoldersWidget(const FoldersWidget&)            = delete;   // copy ctor
        FoldersWidget(FoldersWidget&&)                 = delete;   // move ctor
        FoldersWidget& operator=(const FoldersWidget&) = delete;   // copy assignment
        FoldersWidget& operator=(FoldersWidget&&)      = delete;   // move assignment


    private:
        Ui::FoldersWidget *ui;
    };
}

#endif // GPUI_FOLDERSWIDGET_H
