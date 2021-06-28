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

#include "download_manager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QProcess>

#include <QDebug>

namespace gpui {

class DownloadADMXDialogPrivate {
public:
    static const QString downloadUrl;
    QString downloadDirectory;
    DownloadManager manager;
    QProcess msiextract;
};

const QString DownloadADMXDialogPrivate::downloadUrl
= "https://download.microsoft.com/download/3/0/6/30680643-987a-450c-b906-a455fff4aee8/Administrative%20Templates%20(.admx)%20for%20Windows%2010%20October%202020%20Update.msi";

DownloadADMXDialog::DownloadADMXDialog(QWidget *parent)
    : QDialog(parent)
    , d(new DownloadADMXDialogPrivate())
    , ui(new Ui::DownloadDefinitionsDialog())
{
    ui->setupUi(this);

    setDownloadDirectory(QDir::homePath());

    connect(ui->browseButton, SIGNAL(clicked()), SLOT(onBrowseClick()));
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(onOKClick()));


    connect(&d->manager, &DownloadManager::updateProgress, [&](qint64 bytesReceived, qint64 bytesTotal)
    {
        ui->progressBar->setValue(bytesReceived/bytesTotal);
        ui->progressBar->update();
    });

    connect(&d->manager, &DownloadManager::finished, [&]()
    {
        ui->progressBar->setValue(100);
        ui->progressBar->update();

        unpackMSI(d->manager.getLastDownloadedFileName(), d->downloadDirectory);
    });

    connect(this, &DownloadADMXDialog::folderIsNotEmpty, [&]()
    {
        QMessageBox msg;
        msg.setText("Selected folder is not empty: " + d->downloadDirectory);
        msg.exec();
    });
    connect(this, &DownloadADMXDialog::folderIsNotWriteable, [&]()
    {
        QMessageBox msg;
        msg.setText("Selected folder is not writeable: " + d->downloadDirectory);
        msg.exec();
    });

    connect(&d->msiextract , static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred), [&](QProcess::ProcessError error)
    {
        qWarning() << error;
        ui->progressBar->setValue(100);
        ui->progressBar->update();
    });
}

DownloadADMXDialog::~DownloadADMXDialog()
{
    delete d;

    delete ui;
}

void DownloadADMXDialog::downloadFiles(const QString& url, const QString& outputFolder)
{
    QDir dir(outputFolder);
    if (!(dir.exists()))
    {
        emit folderIsNotWriteable();
        return;
    }

    if (!dir.isEmpty(QDir::NoDotAndDotDot|QDir::AllEntries))
    {
        emit folderIsNotEmpty();
        return;
    }

    d->manager.setOutputDirectory(outputFolder + QDir::separator());
    d->manager.append(url);
}

void DownloadADMXDialog::unpackMSI(const QString& inputMSI, const QString& outputFolder)
{
    ui->progressBar->setValue(50);
    ui->progressBar->update();

    QStringList arguments;
    arguments.append(inputMSI);
    arguments.append("-C");
    arguments.append(outputFolder);
    d->msiextract.startDetached("msiextract", arguments);
    d->msiextract.waitForFinished();

    ui->progressBar->setValue(100);
    ui->progressBar->update();
}

void DownloadADMXDialog::setDownloadDirectory(const QString& directory)
{
    ui->pathLineEdit->setText(directory);
    d->downloadDirectory = directory;
}

void DownloadADMXDialog::onBrowseClick()
{
    QString dir = QFileDialog::getExistingDirectory(
                this,
                tr("Open Directory"),
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    setDownloadDirectory(dir);
}

void DownloadADMXDialog::onOKClick()
{
    downloadFiles(d->downloadUrl, d->downloadDirectory);
}

}
