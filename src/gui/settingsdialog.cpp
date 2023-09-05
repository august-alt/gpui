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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "../core/isettingswidget.h"

namespace gpui
{
SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog())
{
    ui->setupUi(this);
    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);

    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::on_okButtonClicked);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::addTab(ISettingsWidget *settingsWidget)
{
    if (settingsWidget)
    {
        ui->tabWidget->addTab(settingsWidget, settingsWidget->getName());
    }
}

void SettingsDialog::on_okButtonClicked()
{
    for (int i = 0; i < ui->tabWidget->count(); ++i)
    {
        ISettingsWidget *widget = dynamic_cast<ISettingsWidget *>(ui->tabWidget->widget(i));

        if (widget)
        {
            widget->saveSettings();
        }
        else
        {
            qWarning() << "Can't cast to ISettingsWidget fron snapIn!";
        }
    }
}

} // namespace gpui
