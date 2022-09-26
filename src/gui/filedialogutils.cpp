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

#include "filedialogutils.h"

#include <QFileDialog>

#include <memory>

namespace gpui
{
QString FileDialogUtils::getOpenFileName(QWidget *parent, const QString &filter)
{
    std::unique_ptr<QFileDialog> fileDialog = std::make_unique<QFileDialog>(parent);

    fileDialog->setDirectory(QDir::homePath());
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setSupportedSchemes(QStringList(QStringLiteral("file")));

    fileDialog->setLabelText(QFileDialog::Accept, QObject::tr("Open"));
    fileDialog->setLabelText(QFileDialog::FileName, QObject::tr("File name"));
    fileDialog->setLabelText(QFileDialog::LookIn, QObject::tr("Look in"));
    fileDialog->setLabelText(QFileDialog::Reject, QObject::tr("Cancel"));
    fileDialog->setLabelText(QFileDialog::FileType, QObject::tr("File type"));

    fileDialog->setNameFilter(filter);
    fileDialog->setOptions(QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog);

    fileDialog->setWindowTitle(QObject::tr("Open File"));

    if (fileDialog->exec() == QDialog::Accepted)
    {
        return fileDialog->selectedUrls()[0].toLocalFile();
    }

    return QString();
}

QString FileDialogUtils::getOpenDirectoryName(QWidget *parent, const QString &filter)
{
    std::unique_ptr<QFileDialog> fileDialog = std::make_unique<QFileDialog>(parent);

    fileDialog->setDirectory(QDir::homePath());
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setSupportedSchemes(QStringList(QStringLiteral("file")));

    fileDialog->setLabelText(QFileDialog::Accept, QObject::tr("Open"));
    fileDialog->setLabelText(QFileDialog::FileName, QObject::tr("File name"));
    fileDialog->setLabelText(QFileDialog::LookIn, QObject::tr("Look in"));
    fileDialog->setLabelText(QFileDialog::Reject, QObject::tr("Cancel"));
    fileDialog->setLabelText(QFileDialog::FileType, QObject::tr("File type"));

    fileDialog->setNameFilter(filter);
    fileDialog->setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                           | QFileDialog::DontUseNativeDialog);

    fileDialog->setWindowTitle(QObject::tr("Open Directory"));

    if (fileDialog->exec() == QDialog::Accepted)
    {
        return fileDialog->selectedUrls()[0].toLocalFile();
    }

    return QString();
}

} // namespace gpui
