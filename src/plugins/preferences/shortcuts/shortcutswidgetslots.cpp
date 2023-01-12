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

#include "shortcutswidget.h"
#include "ui_shortcutswidget.h"

#include "shortcutsitem.h"

namespace preferences
{
enum ViewMode
{
    CREATE__MODE = 0,
    REPLACE_MODE = 1,
    UPDATE__MODE = 2,
    DELETE__MODE = 3
};

enum TargetTypeMode
{
    FILESYSTEM = 0,
    URL        = 1,
    SHELL      = 2,
};

void ShortcutsWidget::on_actionComboBox_currentIndexChanged(int index)
{
    bool disableWidgets = index == DELETE__MODE;

    ui->targetPathWidget->setDisabled(disableWidgets);
    ui->iconFileWidget->setDisabled(disableWidgets);
    ui->startInWidget->setDisabled(disableWidgets);
}

void ShortcutsWidget::on_locationComboBox_currentIndexChanged(int index)
{
    ui->nameToolButton->setDisabled(index != 0);
}

void ShortcutsWidget::on_nameToolButton_clicked()
{
    ui->nameLineEdit->setText(openFileOrFolder(true));
}

void ShortcutsWidget::on_targetPathToolButton_clicked()
{
    ui->targetPathLineEdit->setText(openFileOrFolder(true));
}

void ShortcutsWidget::on_iconFilePathToolButton_clicked()
{
    ui->iconFilePathLineEdit->setText(openFileOrFolder(false));
}

void ShortcutsWidget::on_startInToolButton_clicked()
{
    ui->startInLineEdit->setText(openFileOrFolder(true));
}

void ShortcutsWidget::on_targetTypeComboBox_currentIndexChanged(int index)
{
    if (index == FILESYSTEM)
    {
        ui->startInLayout->setEnabled(true);
        ui->argumentsLineEdit->setEnabled(true);
        ui->runComboBox->setEnabled(true);
        ui->commentLineEdit->setEnabled(true);
    }

    if (index == URL)
    {
        ui->startInLayout->setEnabled(false);
        ui->argumentsLineEdit->setDisabled(true);
        ui->runComboBox->setDisabled(true);
        ui->commentLineEdit->setDisabled(true);
    }

    if (index == SHELL)
    {
        ui->argumentsLineEdit->setDisabled(true);
        ui->startInLayout->setEnabled(false);
        ui->runComboBox->setEnabled(true);
        ui->commentLineEdit->setEnabled(true);
    }
}

void ShortcutsWidget::on_iconFilePathLineEdit_textChanged(const QString &text)
{
    ui->iconIndexLineEdit->setDisabled(text.isEmpty() || !text.toLower().endsWith(".dll"));
}

QString ShortcutsWidget::openFileOrFolder(bool folderMode)
{
    std::unique_ptr<QFileDialog> fileDialog = std::make_unique<QFileDialog>(this);

    fileDialog->setDirectory(QDir::homePath());
    fileDialog->setSupportedSchemes(QStringList(QStringLiteral("file")));

    fileDialog->setLabelText(QFileDialog::Accept, tr("Open"));
    fileDialog->setLabelText(QFileDialog::FileName, tr("File name"));
    fileDialog->setLabelText(QFileDialog::LookIn, tr("Look in"));
    fileDialog->setLabelText(QFileDialog::Reject, tr("Cancel"));
    fileDialog->setLabelText(QFileDialog::FileType, tr("File type"));

    if (folderMode)
    {
        fileDialog->setFileMode(QFileDialog::Directory);
        fileDialog->setNameFilter(QObject::tr("All files (*.*)"));
        fileDialog->setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                               | QFileDialog::DontUseNativeDialog);
        fileDialog->setWindowTitle(tr("Open Directory"));
    }
    else
    {
        fileDialog->setFileMode(QFileDialog::ExistingFile);
        fileDialog->setNameFilter("Ini Files (*.ini), Dll Files (*.dll), Png Files (*.png)");
        fileDialog->setOptions(QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog);
        fileDialog->setWindowTitle(tr("Open File"));
    }

    if (fileDialog->exec() == QDialog::Accepted)
    {
        return fileDialog->selectedUrls()[0].toLocalFile();
    }

    return "";
}

void ShortcutsWidget::updateCurrentSequence(const QKeySequence &sequence)
{
    if (!m_item)
    {
        qWarning() << "No item found exiting!";

        return;
    }

    m_item->setProperty<std::string>("shortcutKey", sequence.toString().toStdString());
}

void ShortcutsWidget::on_shortkutKeySequenceEdit_editingFinished()
{
    auto keySequence = ui->shortkutKeySequenceEdit->keySequence();

    if (!keySequence.isEmpty())
    {
        int value = ui->shortkutKeySequenceEdit->keySequence()[0];
        QKeySequence shortcut(value);
        ui->shortkutKeySequenceEdit->setKeySequence(shortcut);
    }

    updateCurrentSequence(ui->shortkutKeySequenceEdit->keySequence());
}

} // namespace preferences
