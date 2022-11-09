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

#include "propertieswidgetdelegate.h"

#include <QItemDelegate>
#include <QLabel>

namespace preferences
{
PropertiesWidgetDelegate::PropertiesWidgetDelegate(QWidget *parent)
    : ModelView::ViewModelDelegate(parent)
{}

PropertiesWidgetDelegate::~PropertiesWidgetDelegate() {}

void PropertiesWidgetDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLabel *label = qobject_cast<QLabel *>(editor);
    if (!label || !index.data().canConvert<bool>())
    {
        ModelView::ViewModelDelegate::setEditorData(editor, index);
        return;
    }

    label->setText(index.data().value<bool>() ? QObject::tr("Yes") : QObject::tr("No"));
}

} // namespace preferences
