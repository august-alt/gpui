/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
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

#include "download_admx_dialog.h"
#include "ui_download_admx_dialog.h"

#include <QFileDialog>
#include <QStandardPaths>

#include <QDebug>

namespace gpui {

class DownloadADMXDialogPrivate {
public:
    static const QString downloadUrl;
};

const QString DownloadADMXDialogPrivate::downloadUrl = "";

DownloadADMXDialog::DownloadADMXDialog(QWidget *parent)
    : QDialog(parent)
    , d(new DownloadADMXDialogPrivate())
    , ui(new Ui::DownloadDefinitionsDialog())
{
    ui->setupUi(this);

    ui->pathLineEdit->setPlaceholderText(QDir::homePath());

    connect(ui->browseButton, SIGNAL(clicked()), SLOT(onBrowseClick()));
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(onOKClick()));
}

DownloadADMXDialog::~DownloadADMXDialog()
{
    delete d;

    delete ui;
}

void DownloadADMXDialog::downloadFiles(const QString& url, const QFileInfo& outputFolder)
{
    (void)url;
    (void)outputFolder;

    qDebug() << "Downloading files ...";

    unpackMSI(outputFolder.path() + "file.msi", outputFolder);
}

void DownloadADMXDialog::unpackMSI(const QString& inputMSI, const QFileInfo& outputFolder)
{
    (void)inputMSI;
    (void)outputFolder;

    qDebug() << "Unpacking MSI ...";
}

void DownloadADMXDialog::onBrowseClick()
{
    QString dir = QFileDialog::getExistingDirectory(
                this,
                tr("Open Directory"),
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->pathLineEdit->setText(dir);
}

void DownloadADMXDialog::onOKClick()
{
    downloadFiles(d->downloadUrl, QStandardPaths::standardLocations(QStandardPaths::TempLocation).first());
}

}
