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

#include "registrywidget.h"
#include "ui_registrywidget.h"

namespace preferences
{

const QString DEFAULT_VALUE = "(Default)";

enum ViewMode
{
    CREATE__MODE = 0,
    REPLACE_MODE = 1,
    UPDATE__MODE = 2,
    DELETE__MODE = 3
};

void RegistryWidget::on_actionComboBox_currentIndexChanged(int index)
{
    ui->valueFormLayout->setEnabled(index != DELETE__MODE);
}

void RegistryWidget::on_defaultValueNameCheckBox_stateChanged(int state)
{
    if (state == Qt::CheckState::Checked)
    {
        ui->valueNameLineEdit->setText(DEFAULT_VALUE);
        ui->valueNameLineEdit->setEnabled(false);
    }
    else
    {
        ui->valueNameLineEdit->clear();
        ui->valueNameLineEdit->setEnabled(true);
    }
}

void RegistryWidget::setInitialState()
{
    if (ui->valueNameLineEdit->text().compare(DEFAULT_VALUE) == 0)
    {
        ui->defaultValueNameCheckBox->setChecked(true);
    }
}

}
