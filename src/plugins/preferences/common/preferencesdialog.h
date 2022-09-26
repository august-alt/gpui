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

#ifndef GPUI_PREFERENCES_DIALOG_H
#define GPUI_PREFERENCES_DIALOG_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class PreferencesDialog; }
QT_END_NAMESPACE

#include "widgetfactory.h"

namespace ModelView
{
    class SessionItem;
}

namespace preferences
{
    class PreferencesDialog : public QDialog
    {
        Q_OBJECT

    public:
        PreferencesDialog(ModelView::SessionItem *item, QWidget *parent = 0);
        ~PreferencesDialog();

    private:
        Ui::PreferencesDialog *ui;

    private:
        PreferencesDialog(const PreferencesDialog&)            = delete;   // copy ctor
        PreferencesDialog(PreferencesDialog&&)                 = delete;   // move ctor
        PreferencesDialog& operator=(const PreferencesDialog&) = delete;   // copy assignment
        PreferencesDialog& operator=(PreferencesDialog&&)      = delete;   // move assignment
    };
}

#endif // GPUI_PREFERENCES_DIALOG_H
