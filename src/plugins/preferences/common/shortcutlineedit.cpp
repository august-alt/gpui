/***********************************************************************************************************************
**
** Copyright (C) 2024 BaseALT Ltd. <org@basealt.ru>
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

#include "shortcutlineedit.h"

#include <QEvent>
#include <QKeyEvent>

#include "selectvariabledialog.h"

namespace preferences
{

ShortcutLineEdit::ShortcutLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
}

bool ShortcutLineEdit::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_F3)
        {
            auto dialog = new SelectVariableDialog();

            QObject::connect(dialog, &SelectVariableDialog::variableSelected, [&](QString variable)
            {
                this->insert(variable);
            });

            dialog->exec();

            return true;
        }
    }

    return QWidget::event(event);
}

}
