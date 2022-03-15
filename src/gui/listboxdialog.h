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

#ifndef GPUI_LISTBOX_DIALOG_H
#define GPUI_LISTBOX_DIALOG_H

#include "gui.h"

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ListBoxDialog; }
QT_END_NAMESPACE

namespace gpui {
    class ListBoxDialog : public QDialog
    {
        Q_OBJECT
    public:
        // construction and destruction
        ListBoxDialog(const QString& dialogTitle);
        ~ListBoxDialog();

        void setItems(const QStringList& items);
        void clearItems();

    signals:
        void itemsEditingFinished(QStringList items);

    private:
        Ui::ListBoxDialog *ui;

    private:
        void setupListBoxWidget();

    private slots:
        void onNew();
        void onDelete();

    private:
        ListBoxDialog(const ListBoxDialog&)            = delete;   // copy ctor
        ListBoxDialog(ListBoxDialog&&)                 = delete;   // move ctor
        ListBoxDialog& operator=(const ListBoxDialog&) = delete;   // copy assignment
        ListBoxDialog& operator=(ListBoxDialog&&)      = delete;   // move assignment
    };
}

#endif//GPUI_LISTBOX_DIALOG_H
