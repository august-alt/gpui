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

#include "shareswidget.h"
#include "ui_shareswidget.h"

#include "sharesitem.h"

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

void SharesWidget::on_actionComboBox_currentIndexChanged(int index)
{
    bool deleteMode = index == DELETE__MODE;
    switch (index)
    {
    case ViewMode::CREATE__MODE:
    case ViewMode::REPLACE_MODE:
        ui->modifiersFrame->setDisabled(true);
        ui->userFrame->setEnabled(true);
        ui->accessFrame->setEnabled(true);
        break;
    case ViewMode::UPDATE__MODE:
        ui->modifiersFrame->setEnabled(true);
        ui->userFrame->setEnabled(true);
        ui->accessFrame->setEnabled(true);
        break;
    case ViewMode::DELETE__MODE:
        ui->modifiersFrame->setEnabled(true);
        ui->userFrame->setDisabled(true);
        ui->accessFrame->setDisabled(true);
        break;
    default:
        break;
    }

    ui->commentLineEdit->setDisabled(deleteMode);
    ui->folderToolButton->setDisabled(deleteMode);
    ui->folderPathLineEdit->setDisabled(deleteMode);
}

void SharesWidget::submit()
{
    if (mapper && validate())
    {
        mapper->submit();

        if (ui->noChangeAccess->isChecked())
        {
            m_item->setProperty(preferences::SharesItem::ACCESS_BASED_ENUMERATION, "NO_CHANGE");
        }

        if (ui->enableAccess->isChecked())
        {
            m_item->setProperty(preferences::SharesItem::ACCESS_BASED_ENUMERATION, "ENABLE");
        }

        if (ui->disableAccess->isChecked())
        {
            m_item->setProperty(preferences::SharesItem::ACCESS_BASED_ENUMERATION, "DISABLE");
        }

        if (ui->noChangeUsers->isChecked())
        {
            m_item->setProperty(preferences::SharesItem::LIMIT_USERS, "NO_CHANGE");
        }

        if (ui->maximumAllowedUsers->isChecked())
        {
            m_item->setProperty(preferences::SharesItem::LIMIT_USERS, "MAX_ALLOWED");
        }

        if (ui->allowThisNumberOfUsers->isChecked())
        {
            m_item->setProperty(preferences::SharesItem::LIMIT_USERS, "SET_LIMIT");
        }

        emit dataChanged();
    }
}

void SharesWidget::on_noChangeUsers_clicked()
{
    ui->numberOfUsers->setDisabled(true);
}

void SharesWidget::setInitialUserFrameRadioButton(const QString current)
{
    if (current.compare("NO_CHANGE") == 0)
    {
        ui->noChangeUsers->setChecked(true);
    }

    if (current.compare("MAX_ALLOWED") == 0)
    {
        ui->maximumAllowedUsers->setChecked(true);
    }

    if (current.compare("SET_LIMIT") == 0)
    {
        ui->allowThisNumberOfUsers->setChecked(true);
    }
}

void SharesWidget::setInitialAccessFrameRadioButton(const QString current)
{
    if (current.compare("NO_CHANGE") == 0)
    {
        ui->noChangeAccess->setChecked(true);
    }

    if (current.compare("ENABLE") == 0)
    {
        ui->enableAccess->setChecked(true);
    }

    if (current.compare("DISABLE") == 0)
    {
        ui->disableAccess->setChecked(true);
    }
}

void SharesWidget::on_maximumAllowedUsers_clicked()
{
    ui->numberOfUsers->setDisabled(true);
}

void SharesWidget::on_allowThisNumberOfUsers_clicked()
{
    ui->numberOfUsers->setEnabled(true);
}

void SharesWidget::on_folderToolButton_clicked()
{
    using namespace gpui;

    QString newText;
    if (FileDialogUtils::getOpenDirectoryName(newText, this, QObject::tr("All files (*.*)")))
    {
        ui->folderPathLineEdit->setText(newText);
    }
}

}
