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

#include "iniwidget.h"
#include "ui_iniwidget.h"

#include "gui/filedialogutils.h"

namespace preferences
{

enum ViewMode
{
    CREATE__MODE = 0,
    REPLACE_MODE = 1,
    UPDATE__MODE = 2,
    DELETE__MODE = 3
};

void IniWidget::on_actionComboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case ViewMode::CREATE__MODE:
    case ViewMode::UPDATE__MODE:
    case ViewMode::REPLACE_MODE:
        if (!ui->propertyLineEdit->text().isEmpty())
        {
            ui->valueLineEdit->setEnabled(true);
        }
        break;
    case ViewMode::DELETE__MODE:
        ui->valueLineEdit->setEnabled(false);
        ui->valueLineEdit->clear();
        break;
    default:
        break;
    }
}


void IniWidget::on_pathToolButton_clicked()
{
    using namespace gpui;

    ui->pathLineEdit->setText(FileDialogUtils::getOpenFileName(this, QObject::tr("Ini files (*.ini)")));
}

void IniWidget::on_pathLineEdit_textChanged(const QString &text)
{
    bool disabled = text.isEmpty();

    ui->sectionLineEdit->setDisabled(disabled);
    if (disabled)
    {
        ui->propertyLineEdit->setDisabled(disabled);
        ui->valueLineEdit->setDisabled(disabled);
    }
}

void IniWidget::on_sectionLineEdit_textChanged(const QString &text)
{
    bool disabled = text.isEmpty();

    ui->propertyLineEdit->setDisabled(disabled);
    if (disabled)
    {
        ui->valueLineEdit->setDisabled(disabled);
    }
}

void IniWidget::on_propertyLineEdit_textChanged(const QString &text)
{
    bool disabled = text.isEmpty();

    ui->valueLineEdit->setDisabled(disabled);
}

}
