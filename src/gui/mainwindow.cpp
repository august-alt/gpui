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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindowsettings.h"

#include "contentwidget.h"

#include "../model/bundle/policybundle.h"

#include "../model/registry/registry.h"
#include "../model/registry/polregistrysource.h"

#include "../io/registryfileformat.h"

#include "../io/genericreader.h"
#include "../io/registryfile.h"
#include "../io/registryfileformat.h"

#include "smbfilebrowser.h"

#include <libnemofolderlistmodel/qsambaclient/smblocationitemfile.h>

namespace gpui {

class MainWindowPrivate {
public:
    std::unique_ptr<QStandardItemModel> model = nullptr;
    ContentWidget* contentWidget;
    std::unique_ptr<MainWindowSettings> settings = nullptr;

    std::unique_ptr<model::registry::AbstractRegistrySource> userRegistrySource = nullptr;
    std::shared_ptr<model::registry::Registry> userRegistry = nullptr;

    std::unique_ptr<model::registry::AbstractRegistrySource> machineRegistrySource = nullptr;
    std::shared_ptr<model::registry::Registry> machineRegistry = nullptr;
};

void save(const std::string &fileName, std::shared_ptr<model::registry::Registry> registry)
{
    std::unique_ptr<io::RegistryFile> fileData = std::make_unique<io::RegistryFile>();
    fileData->setRegistry(registry);

    QString pluginName = QString::fromStdString(fileName);
    pluginName = pluginName.mid(pluginName.lastIndexOf('.') + 1);

    io::RegistryFileFormat<io::RegistryFile>* format = gpui::PluginStorage::instance()->createPluginClass<io::RegistryFileFormat<io::RegistryFile> >(pluginName);

    if (!format)
    {
        qWarning() << "Format supporting: " << pluginName << " not found.";

        return;
    }

    std::ofstream file;

    file.open(fileName, std::ofstream::out | std::ofstream::binary);

    if (file.good()) {
        if (!format->write(file, fileData.get()))
        {
            qWarning() << fileName.c_str() << " " << format->getErrorString().c_str();
        }
    }

    file.close();

    delete format;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate())
    , ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    d->contentWidget = new ContentWidget(this);
    ui->splitter->addWidget(d->contentWidget);

    d->settings = std::make_unique<MainWindowSettings>(this, ui);
    d->settings->restoreSettings();

    connect(ui->actionOpenPolicyDirectory, &QAction::triggered, this, &MainWindow::onDirectoryOpen);
    connect(ui->actionOpenUserRegistrySource, &QAction::triggered, this, &MainWindow::onUserRegistrySourceOpen);
    connect(ui->actionOpenMachineRegistrySource, &QAction::triggered, this, &MainWindow::onMachineRegistrySourceOpen);
    connect(ui->actionSaveRegistrySource, &QAction::triggered, this, &MainWindow::onRegistrySourceSave);
    connect(ui->treeView, &QTreeView::clicked, d->contentWidget, &ContentWidget::modelItemSelected);
}

MainWindow::~MainWindow()
{
    delete d;

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    d->settings->saveSettings();

    QMainWindow::closeEvent(event);
}

void MainWindow::onDirectoryOpen()
{
    QString directory = QFileDialog::getExistingDirectory(
                        this,
                        tr("Open Directory"),
                        QDir::homePath(),
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    auto bundle = std::make_unique<model::bundle::PolicyBundle>();
    d->model = bundle->loadFolder(directory.toStdString(), "ru-ru");

    ui->treeView->setModel(d->model.get());
    d->contentWidget->setModel(d->model.get());

    d->contentWidget->setSelectionModel(ui->treeView->selectionModel());
}

void MainWindow::onUserRegistrySourceOpen()
{
    onRegistrySourceOpen(d->userRegistry, d->userRegistrySource,
                         [&](model::registry::AbstractRegistrySource* source)
    {
        d->contentWidget->setUserRegistrySource(source);
    });
}

void MainWindow::onMachineRegistrySourceOpen()
{
    onRegistrySourceOpen(d->machineRegistry, d->machineRegistrySource,
                         [&](model::registry::AbstractRegistrySource* source)
    {
        d->contentWidget->setMachineRegistrySource(source);
    });
}

void MainWindow::onRegistrySourceSave()
{
    QString polFileName = QFileDialog::getSaveFileName(
                        this,
                        tr("Open Directory"),
                        QDir::homePath(),
                        "*.pol");

    save(polFileName.toStdString(), d->userRegistry);
}

void MainWindow::onRegistrySourceOpen(std::shared_ptr<model::registry::Registry>& registry,
                                      std::unique_ptr<model::registry::AbstractRegistrySource>& source,
                                      std::function<void(model::registry::AbstractRegistrySource* source)> callback)
{
    SmbFileBrowser browser(this);

    connect(&browser, &SmbFileBrowser::onPolOpen, this, [&](const QString& path)
    {
        qWarning() << "Path recieved: " << path;

        auto stringvalues = std::make_unique<std::string>();

        if (path.startsWith("smb://"))
        {
            SmbLocationItemFile smbLocationItemFile(path);
            smbLocationItemFile.open(QFile::ReadWrite);
            stringvalues->resize(smbLocationItemFile.size(), 0);
            smbLocationItemFile.read(&stringvalues->at(0), smbLocationItemFile.size());
        }
        else
        {
            QFile registryFile(path);
            registryFile.open(QFile::ReadWrite);
            stringvalues->resize(registryFile.size(), 0);
            registryFile.read(&stringvalues->at(0), registryFile.size());
        }

        auto iss = std::make_unique<std::istringstream>(*stringvalues);
        std::string pluginName("pol");

        auto reader = std::make_unique<io::GenericReader>();
        auto registryFile = reader->load<io::RegistryFile, io::RegistryFileFormat<io::RegistryFile> >(*iss, pluginName);
        if (!registryFile)
        {
            qWarning() << "Unable to load registry file contents.";
            return;
        }

        registry = registryFile->getRegistry();

        source = std::make_unique<model::registry::PolRegistrySource>(registry);

        callback(source.get());
    });

    browser.exec();
}

}
