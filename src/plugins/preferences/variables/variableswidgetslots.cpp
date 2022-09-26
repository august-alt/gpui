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

#include "variableswidget.h"
#include "ui_variableswidget.h"

namespace preferences
{

const QString PATH = "PATH";

enum ViewMode
{
    CREATE__MODE = 0,
    REPLACE_MODE = 1,
    UPDATE__MODE = 2,
    DELETE__MODE = 3
};

void VariablesWidget::on_actionComboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case ViewMode::CREATE__MODE:
    case ViewMode::UPDATE__MODE:
    case ViewMode::REPLACE_MODE:
        ui->valueLineEdit->setEnabled(true);
        break;
    case ViewMode::DELETE__MODE:
        ui->valueLineEdit->setEnabled(false);
        break;
    default:
        break;
    }

    setDetailsText(index);
}

void VariablesWidget::on_pathCheckBox_stateChanged(int state)
{
    if (state == Qt::CheckState::Checked)
    {
        ui->nameLineEdit->setText(PATH);
        ui->nameLineEdit->setDisabled(true);

        ui->partialCheckBox->setEnabled(true);
        ui->partialCheckBox->setChecked(true);
    }
    else
    {
        ui->nameLineEdit->clear();
        ui->nameLineEdit->setEnabled(true);

        ui->partialCheckBox->setEnabled(false);
        ui->partialCheckBox->setChecked(false);
    }
}

void VariablesWidget::on_userVariableRadio_clicked()
{
    ui->partialCheckBox->setChecked(false);
    ui->pathCheckBox->setChecked(false);

    ui->partialCheckBox->setDisabled(true);
    ui->pathCheckBox->setDisabled(true);
}

void VariablesWidget::on_systemVariableRadio_clicked()
{
    ui->pathCheckBox->setEnabled(true);
}

void VariablesWidget::setDetailsText(int index)
{
    bool pathMode = ui->pathCheckBox->isChecked();
    bool partialMode = ui->partialCheckBox->isChecked();

    switch (index)
    {
    case ViewMode::CREATE__MODE:
        if (pathMode)
        {
            if (partialMode)
            {
                ui->detailsLabel->setText(tr("Create will add the variable to the specified environment."
                                             " If it already exists, the value will not be changed."));
            }
            else
            {
                ui->detailsLabel->setText(tr("The PATH variable contains a list of semicolon delimited"
                                             " folder paths that Windows uses when locating files."
                                             " Create will create the full path variable as specified if"
                                             " it does not exist. Generally the path will already exist."));
            }
        }
        else
        {
            ui->detailsLabel->setText(tr("Create will add the variable to the specified environment."
                                         " If it already exists, the value will not be changed."));
        }
        break;
    case ViewMode::UPDATE__MODE:
        if (pathMode)
        {
            if (partialMode)
            {
                ui->detailsLabel->setText(tr("The PATH variable contains a list of semicolon delimited folder paths"
                                             " that Windows uses when locating files. Update will replace the specified"
                                             " part of the path variable. This will have no net effect on the path with"
                                             " the possible exception of changing the text case of the specified"
                                             " segment. Multiple segments are not supported by this option."));
            }
            else
            {
                ui->detailsLabel->setText(tr("The PATH variable contains a list of semicolon delimited folder paths"
                                             " that Windows uses when locating files. Update will replace the FULL"
                                             " path variable as specified. This will remove all previously existing"
                                             " path values."));
            }
        }
        else
        {
            ui->detailsLabel->setText(tr("Update will replace the variable in the specified environment."
                                         " If it doesn't exist, it will be created."));
        }
        break;
    case ViewMode::REPLACE_MODE:
        if (pathMode)
        {
            if (partialMode)
            {
                ui->detailsLabel->setText(tr("The PATH variable contains a list of semicolon delimited folder"
                                             " paths that Windows uses when locating files. Replace will replace"
                                             " the specified part of the path variable. This will have no net"
                                             " effect on the path with the possible exception of changing the"
                                             " text case of the specified segment. Multiple segments are not"
                                             " supported by this option."));
            }
            else
            {
                ui->detailsLabel->setText(tr("The PATH variable contains a list of semicolon delimited folder paths that"
                                             " Windows uses when locating files. Replace will replace the FULL path"
                                             " variable as specified. This will remove all previously existing"
                                             " path values."));
            }
        }
        else
        {
            ui->detailsLabel->setText(tr("Replace will replace the variable in the specified environment."
                                         " If it doesn't exist, it will be created."));
        }
        break;
    case ViewMode::DELETE__MODE:
        if (pathMode)
        {
            if (partialMode)
            {
                ui->detailsLabel->setText(tr("The PATH variable contains a list of semicolon delimited folder paths"
                                             " that Windows uses when locating files. Delete will remove the specified"
                                             " segment from the existing path variable. If the segment doesn't exist,"
                                             " an error is not returned. Multiple segments are not supported"
                                             " by this option."));
            }
            else
            {
                ui->detailsLabel->setText(tr("The PATH variable contains a list of semicolon delimited folder paths"
                                             " that Windows uses when locating files. Delete will delete the FULL path"
                                             " variable. If it doesn't exist, an error is not returned. This"
                                             " setting is not generally recommended."));
            }
        }
        else
        {
            ui->detailsLabel->setText(tr("Delete will remove the variable from the specified environment."
                                         " If it doesn't exist, an error is not returned."));
        }
        break;
    default:
        break;
    }
}

void VariablesWidget::setDefaultValues()
{
    if (ui->nameLineEdit->text().compare(PATH) == 0)
    {
        ui->pathCheckBox->setChecked(true);
    }
}

}
