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

#include "driveswidget.h"
#include "ui_driveswidget.h"

#include <array>

#include "drivesitem.h"

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

void DrivesWidget::on_actionComboBox_currentIndexChanged(int index)
{
    bool deleteMode = index == ViewMode::DELETE__MODE;

    ui->locationFrame->setDisabled(deleteMode);
    ui->thisGroupBox->setDisabled(deleteMode);

    ui->firstAvailableRadioButton->setText(deleteMode ? "Delete all, starting at:"
                                                      : "Use first available, starting at:");

    setDriveRadioButtonText(deleteMode ? DrivesWidgetMode::DELETE
                            : (ui->pathLineEdit->text().isEmpty() && index == ViewMode::UPDATE__MODE)
                                ? DrivesWidgetMode::USE_EXISTING
                                : DrivesWidgetMode::CREATE);

    disableLetterWidgets(ui->pathLineEdit->text(), ui->actionComboBox->currentIndex());
}

void DrivesWidget::submit()
{
    if (mapper && validate())
    {
        mapper->submit();

        if (m_item)
        {
            m_item->setProperty<int>(DrivesItem::THIS_DRIVE, ui->thisButtonGroup->checkedId());
            m_item->setProperty<int>(DrivesItem::ALL_DRIVES, ui->allButtonGroup->checkedId());
        }

        emit dataChanged();
    }
}

void DrivesWidget::on_pathToolButton_clicked()
{
    using namespace gpui;

    ui->pathLineEdit->setText(FileDialogUtils::getOpenDirectoryName(this, QObject::tr("All files (*.*)")));
}

void DrivesWidget::setDriveRadioButtonText(DrivesWidgetMode mode)
{
    switch (mode)
    {
    case DrivesWidgetMode::CREATE:
        ui->driveRadioButton->setText("Use:");
        break;
    case DrivesWidgetMode::USE_EXISTING:
        ui->driveRadioButton->setText("Existing:");
        break;
    case DrivesWidgetMode::DELETE:
        ui->driveRadioButton->setText("Delete:");
        break;
    default:
        break;
    }
}

void DrivesWidget::disableLetterWidgets(const QString &text, int index)
{
    bool createOrReplace = (index == ViewMode::CREATE__MODE) || (index == ViewMode::REPLACE_MODE);
    bool disableWidgets  = text.isEmpty() && createOrReplace;

    ui->driveLetterGroupBox->setDisabled(disableWidgets);
    if (index != ViewMode::DELETE__MODE)
    {
        ui->thisGroupBox->setDisabled(disableWidgets);
    }

    ui->firstAvailableRadioButton->setDisabled(text.isEmpty() && index == ViewMode::UPDATE__MODE);
}

void DrivesWidget::setThisDriveCheckBox(const int index)
{
    auto button = ui->thisButtonGroup->button(index); //->setChecked(true);
    qWarning() << button;
}

void DrivesWidget::setAllDrivesCheckBox(const int index)
{
    ui->allButtonGroup->button(index)->setChecked(true);
}

void DrivesWidget::on_pathLineEdit_textChanged(const QString &text)
{
    int index = ui->actionComboBox->currentIndex();
    disableLetterWidgets(text, index);

    setDriveRadioButtonText(text.isEmpty() && index == ViewMode::UPDATE__MODE ? DrivesWidgetMode::USE_EXISTING
                                                                              : DrivesWidgetMode::CREATE);
}

} // namespace preferences
