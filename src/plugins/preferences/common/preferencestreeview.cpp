/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
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

#include "preferencestreeview.h"

#include <QPainter>

namespace preferences
{
PreferencesTreeView::PreferencesTreeView(QWidget *parent)
    : QTreeView(parent)
{}

void PreferencesTreeView::paintEvent(QPaintEvent *event)
{
    if (model() && model()->rowCount() > 0)
    {
        QTreeView::paintEvent(event);
    }
    else
    {
        // If no items draw a text in the center of the viewport.
        QPainter painter(viewport());
        QString text(tr("There are no policies currently"));
        QRect textRect = painter.fontMetrics().boundingRect(text);
        textRect.moveTo(viewport()->rect().center().x() - (textRect.width() * 0.5), 0);
        painter.drawText(textRect, Qt::AlignCenter, text);
    }
}

} // namespace preferences
