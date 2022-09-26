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

#ifndef GPUI_PREFERENCES_WIDGET_INTERFACE_H
#define GPUI_PREFERENCES_WIDGET_INTERFACE_H

#include <QtWidgets>

namespace ModelView
{
    class SessionItem;
}

namespace preferences
{
    class PreferenceWidgetInterface : public QWidget
    {
    public:
        Q_OBJECT

    public:
        PreferenceWidgetInterface(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
            : QWidget(parent, f)
        {}

        virtual ~PreferenceWidgetInterface() = default;

        virtual void setItem(ModelView::SessionItem *item) = 0;

        virtual bool validate() = 0;

        virtual QString name() const = 0;

    signals:
        void dataChanged();

    public slots:
        virtual void submit() = 0;
    };
}

#endif//GPUI_PREFERENCES_WIDGET_INTERFACE_H
