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

#include "aboutdialog.h"

#include "commandlineoptions.h"

#include "contentwidget.h"

#include "../model/bundle/policybundle.h"

#include "../model/registry/registry.h"
#include "../model/registry/polregistrysource.h"

#include "../io/registryfileformat.h"

#include "../io/genericreader.h"
#include "../io/registryfile.h"
#include "../io/registryfileformat.h"

#include "../plugins/storage/smb/smbfile.h"

void registerResources()
{
    Q_INIT_RESOURCE(translations);
}

namespace gpui {

class MainWindowPrivate {
public:
    std::unique_ptr<QStandardItemModel> model = nullptr;
    ContentWidget* contentWidget;
    std::unique_ptr<MainWindowSettings> settings = nullptr;

    std::shared_ptr<model::registry::Registry> userRegistry;
    std::unique_ptr<model::registry::AbstractRegistrySource> userRegistrySource;
    QString userRegistryPath;

    std::shared_ptr<model::registry::Registry> machineRegistry;
    std::unique_ptr<model::registry::AbstractRegistrySource> machineRegistrySource;
    QString machineRegistryPath;

    std::unique_ptr<QSortFilterProxyModel> sortModel = nullptr;

    std::vector<std::unique_ptr<QTranslator>> translators;
    QString localeName;

    CommandLineOptions options;

    MainWindowPrivate()
        : userRegistry(new model::registry::Registry())
        , userRegistrySource(new model::registry::PolRegistrySource(userRegistry))
        , machineRegistry(new model::registry::Registry())
        , machineRegistrySource(new model::registry::PolRegistrySource(machineRegistry))
    {}

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

    auto oss = std::make_unique<std::ostringstream>();

    if (!format->write(*oss, fileData.get()))
    {
        qWarning() << fileName.c_str() << " " << format->getErrorString().c_str();
    }

    oss->flush();

    qWarning() << "Current string values." << oss->str().c_str();

    if (QString::fromStdString(fileName).startsWith("smb://"))
    {
        gpui::smb::SmbFile smbLocationItemFile(QString::fromStdString(fileName));
        bool isOpen = smbLocationItemFile.open(QFile::WriteOnly | QFile::Truncate);
        if (!isOpen)
        {
            isOpen = smbLocationItemFile.open(QFile::NewOnly | QFile::WriteOnly);
        }
        if (isOpen && oss->str().size() > 0)
        {
            smbLocationItemFile.write(&oss->str().at(0), oss->str().size());
        }
        smbLocationItemFile.close();
    }
    else
    {
        QFile registryFile(QString::fromStdString(fileName));
        registryFile.open(QFile::WriteOnly | QFile::Truncate);
        if (!registryFile.isOpen())
        {
            registryFile.open(QFile::NewOnly | QFile::WriteOnly);
        }
        if (registryFile.isOpen() && registryFile.isWritable() && oss->str().size() > 0)
        {
            registryFile.write(&oss->str().at(0), oss->str().size());
        }
        registryFile.close();
    }


    delete format;
}

MainWindow::MainWindow(CommandLineOptions &options, QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate())
    , ui(new Ui::MainWindow())
{
    registerResources();

    QLocale locale;
    std::unique_ptr<QTranslator> qtTranslator = std::make_unique<QTranslator>();
    qtTranslator->load(locale, "gui", "_", ":/");
    QCoreApplication::installTranslator(qtTranslator.get());
    d->translators.push_back(std::move(qtTranslator));
    d->localeName = locale.name().replace("_", "-");

    d->options = options;

    ui->setupUi(this);

    createLanguageMenu();

    d->contentWidget = new ContentWidget(this);
    d->contentWidget->setMachineRegistrySource(d->machineRegistrySource.get());
    d->contentWidget->setUserRegistrySource(d->userRegistrySource.get());

    ui->splitter->addWidget(d->contentWidget);

    d->settings = std::make_unique<MainWindowSettings>(this, ui);
    d->settings->restoreSettings();

    connect(ui->actionOpenPolicyDirectory, &QAction::triggered, this, &MainWindow::onDirectoryOpen);
    connect(ui->actionSaveRegistrySource, &QAction::triggered, this, &MainWindow::onRegistrySourceSave);
    connect(ui->treeView, &QTreeView::clicked, d->contentWidget, &ContentWidget::modelItemSelected);

    QGuiApplication::setWindowIcon(QIcon(":gpui.png"));

    if (d->options.policyBundle.isEmpty())
    {
        d->options.policyBundle = "/usr/share/PolicyDefinitions";
    }

    loadPolicyBundleFolder(d->options.policyBundle, d->localeName);

    if (!d->options.path.isEmpty())
    {
        d->userRegistryPath = d->options.path + "/User/Registry.pol";
        d->machineRegistryPath = d->options.path + "/Machine/Registry.pol";

        onPolFileOpen(d->userRegistryPath, d->userRegistry, d->userRegistrySource,
                      [&](model::registry::AbstractRegistrySource* source)
        {
            d->contentWidget->setUserRegistrySource(source);
        });

        onPolFileOpen(d->machineRegistryPath, d->machineRegistry, d->machineRegistrySource,
                      [&](model::registry::AbstractRegistrySource* source)
        {
            d->contentWidget->setMachineRegistrySource(source);
        });
    }

    connect(d->contentWidget, &ContentWidget::savePolicyChanges, this, &MainWindow::onRegistrySourceSave);
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

void gpui::MainWindow::loadPolicyBundleFolder(const QString& path, const QString &locale)
{
    auto bundle = std::make_unique<model::bundle::PolicyBundle>();
    d->model = bundle->loadFolder(path.toStdString(), locale.toStdString());

    d->sortModel = std::make_unique<QSortFilterProxyModel>();
    d->sortModel->setSourceModel(d->model.get());
    d->sortModel->setSortLocaleAware(true);
    d->sortModel->setSortRole(Qt::DisplayRole);
    d->sortModel->sort(0);

    ui->treeView->setModel(d->sortModel.get());
    d->contentWidget->setModel(d->sortModel.get());

    d->contentWidget->setSelectionModel(ui->treeView->selectionModel());

    d->contentWidget->modelItemSelected(d->sortModel->index(0, 0));
}

void MainWindow::onDirectoryOpen()
{    

    std::unique_ptr<QFileDialog> fileDialog = std::make_unique<QFileDialog>(this);

    fileDialog->setDirectory(QDir::homePath());
    fileDialog->setFileMode(QFileDialog::Directory);

    fileDialog->setLabelText(QFileDialog::Accept, tr("Open"));
    fileDialog->setLabelText(QFileDialog::FileName, tr("File name"));
    fileDialog->setLabelText(QFileDialog::LookIn, tr("Look in"));
    fileDialog->setLabelText(QFileDialog::Reject, tr("Cancel"));

    fileDialog->setNameFilter(QObject::tr("All files (*.*)"));
    fileDialog->setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    fileDialog->setWindowTitle(tr("Open Directory"));

    if (fileDialog->exec() == QDialog::Accepted)
    {
        d->options.policyBundle = fileDialog->selectedUrls().value(0).toLocalFile();
    }

    loadPolicyBundleFolder(d->options.policyBundle, d->localeName);
}

void MainWindow::onRegistrySourceSave()
{
    if (!d->machineRegistryPath.isEmpty())
    {
        qWarning() << "Saving machine registry to: " << d->machineRegistryPath;
        save(d->machineRegistryPath.toStdString(), d->machineRegistry);
    }
    else
    {
        qWarning() << "Unable to save machine registry path is empty!";
    }

    if (!d->userRegistryPath.isEmpty())
    {
        qWarning() << "Saving user registry to: " << d->userRegistryPath;
        save(d->userRegistryPath.toStdString(), d->userRegistry);
    }
    else
    {
        qWarning() << "Unable to save user registry path is empty!";
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    auto about = new AboutDialog(this);
    about->show();
}

void MainWindow::onLanguageChanged(QAction *action)
{
    for (const auto& translator : d->translators)
    {
        qApp->removeTranslator(translator.get());
    }
    d->translators.clear();

    QString language = action->data().toString();

    std::unique_ptr<QTranslator> qtTranslator = std::make_unique<QTranslator>();
    bool loadResult = qtTranslator->load("gui_" + language + ".qm", ":/");
    QCoreApplication::installTranslator(qtTranslator.get());
    d->translators.push_back(std::move(qtTranslator));
    qWarning() << "Load language " << language << loadResult;

    QLocale locale(language);

    d->localeName = locale.name().replace("_", "-");

    loadPolicyBundleFolder(d->options.policyBundle, d->localeName);

    d->contentWidget->onLanguageChaged();
    ui->retranslateUi(this);

    ui->treeView->selectionModel()->clearSelection();
}

void MainWindow::onPolFileOpen(const QString &path,
                               std::shared_ptr<model::registry::Registry> &registry,
                               std::unique_ptr<model::registry::AbstractRegistrySource> &source,
                               std::function<void (model::registry::AbstractRegistrySource *)> callback)
{
    qWarning() << "Path recieved: " << path;

    auto stringvalues = std::make_unique<std::string>();

    try {

    if (path.startsWith("smb://"))
    {
        gpui::smb::SmbFile smbLocationItemFile(path);
        smbLocationItemFile.open(QFile::ReadOnly);
        stringvalues->resize(smbLocationItemFile.size(), 0);
        smbLocationItemFile.read(&stringvalues->at(0), smbLocationItemFile.size());
        smbLocationItemFile.close();
    }
    else
    {
        QFile registryFile(path);
        registryFile.open(QFile::ReadWrite);
        stringvalues->resize(registryFile.size(), 0);
        registryFile.read(&stringvalues->at(0), registryFile.size());
        registryFile.close();
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
    }
    catch (std::exception& e)
    {
        qWarning() << "Unable to read file: " << qPrintable(path) << " Error: " << e.what();
    }
}

void MainWindow::createLanguageMenu()
{
    QActionGroup* langGroup = new QActionGroup(this);
    langGroup->setExclusive(true);

    connect(langGroup, &QActionGroup::triggered, this, &MainWindow::onLanguageChanged);

    QString defaultLocale = QLocale::system().name().left(QLocale::system().name().lastIndexOf('_'));
    QDir dir(":/");
    QStringList fileNames = dir.entryList(QStringList("gui_*.qm"));

    QMenu* menu = new QMenu(this);
    ui->actionLanguage->setMenu(menu);

    for (QString locale : fileNames)
    {
        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.lastIndexOf('_') + 1);

        QString language = QLocale::languageToString(QLocale(locale).language());

        QAction *action = new QAction(language, this);
        action->setCheckable(true);
        action->setData(locale);

        menu->addAction(action);
        langGroup->addAction(action);

        if (defaultLocale == locale)
        {
            action->setChecked(true);
        }
    }
}

}
