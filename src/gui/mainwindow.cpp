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
#include "mainwindowsettings.h"
#include "ui_mainwindow.h"

#include "aboutdialog.h"

#include "commandlineoptions.h"

#include "contentwidget.h"

#include "treevieweventfilter.h"

#include "../core/isnapin.h"
#include "../core/isnapinmanager.h"

#include "../plugins/administrative_templates/bundle/policybundle.h"

#include "../plugins/administrative_templates/registry/polregistrysource.h"
#include "../plugins/administrative_templates/registry/registry.h"

#include "../io/registryfileformat.h"

#include "../io/genericreader.h"
#include "../io/registryfile.h"
#include "../io/registryfileformat.h"

#include "../io/policydefinitionsfile.h"
#include "../io/policyfileformat.h"

#include "../io/inifile.h"

#include "../plugins/storage/smb/smbfile.h"

#include "../plugins/administrative_templates/bundle/policyroles.h"

void registerResources()
{
    Q_INIT_RESOURCE(translations);
}

namespace gpui
{
class MainWindowPrivate
{
public:
    std::unique_ptr<QAbstractItemModel> model    = nullptr;
    ContentWidget *contentWidget                 = nullptr;
    std::unique_ptr<MainWindowSettings> settings = nullptr;
    ISnapInManager *manager                      = nullptr;

    std::unique_ptr<QSortFilterProxyModel> itemNameSortModel = nullptr;
    std::unique_ptr<QSortFilterProxyModel> itemRoleSortModel = nullptr;
    std::unique_ptr<QSortFilterProxyModel> searchModel       = nullptr;

    std::vector<std::unique_ptr<QTranslator>> translators{};
    QString localeName{};

    QString itemName{};

    QIcon windowIcon{};

    CommandLineOptions options{};

    std::unique_ptr<TreeViewEventFilter> eventFilter = nullptr;

    MainWindowPrivate()
        : eventFilter(new TreeViewEventFilter())
    {}

private:
    MainWindowPrivate(const MainWindowPrivate &) = delete;            // copy ctor
    MainWindowPrivate(MainWindowPrivate &&)      = delete;            // move ctor
    MainWindowPrivate &operator=(const MainWindowPrivate &) = delete; // copy assignment
    MainWindowPrivate &operator=(MainWindowPrivate &&) = delete;      // move assignment
};

MainWindow::MainWindow(CommandLineOptions &options, ISnapInManager *manager, QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate())
    , ui(new Ui::MainWindow())
{
    registerResources();

    d->manager = manager;

    d->options = options;

    ui->setupUi(this);

    ui->treeView->installEventFilter(d->eventFilter.get());

    d->settings = std::make_unique<MainWindowSettings>(this, ui);
    d->settings->restoreSettings();

    createLanguageMenu();

    d->contentWidget = new ContentWidget(this);
    d->contentWidget->setEventFilter(d->eventFilter.get());

    ui->splitter->addWidget(d->contentWidget);

    connect(d->eventFilter.get(), &TreeViewEventFilter::onEnter, this, [&]() {
        ui->treeView->clicked(ui->treeView->currentIndex());
    });

    connect(ui->actionOpenPolicyDirectory, &QAction::triggered, this, &MainWindow::onDirectoryOpen);
    connect(ui->actionSaveRegistrySource, &QAction::triggered, this, &MainWindow::onRegistrySourceSave);
    connect(ui->treeView, &QTreeView::clicked, d->contentWidget, &ContentWidget::modelItemSelected);
    connect(ui->treeView, &QTreeView::clicked, [&](const QModelIndex &index) { d->itemName = index.data().toString(); });

    QLocale locale(!d->localeName.trimmed().isEmpty() ? d->localeName.replace("-", "_")
                                                      : QLocale::system().name().replace("-", "_"));
    std::unique_ptr<QTranslator> qtTranslator = std::make_unique<QTranslator>();
    qtTranslator->load(locale, "gui", "_", ":/");
    QCoreApplication::installTranslator(qtTranslator.get());
    d->translators.push_back(std::move(qtTranslator));
    d->localeName = locale.name().replace("_", "-");
    d->contentWidget->onLanguageChanged();
    ui->retranslateUi(this);

    d->windowIcon = QIcon(":gpui.png");

    QGuiApplication::setWindowIcon(d->windowIcon);

    if (d->options.policyBundle.isEmpty())
    {
        d->options.policyBundle = "/usr/share/PolicyDefinitions";
    }

    for (auto &snapIn : manager->getSnapIns())
    {
        qWarning() << "Loading model from: " << snapIn->getDisplayName();
        snapIn->onInitialize();
        loadPolicyModel(snapIn, d->localeName);
    }

    if (!d->options.path.isEmpty())
    {
        for (auto &snapIn : manager->getSnapIns())
        {
            snapIn->onDataLoad(d->options.path.toStdString(), d->localeName.toStdString());
        }
    }

    if (!d->options.policyName.isEmpty())
    {
        setWindowTitle("GPUI - " + d->options.policyName);
    }

    //    connect(d->contentWidget, &ContentWidget::savePolicyChanges, this, &MainWindow::onRegistrySourceSave);

    connect(ui->searchLineEdit, &QLineEdit::textChanged, [&](const QString &text) {
        d->searchModel->setFilterFixedString(text);
    });
}

MainWindow::~MainWindow()
{
    delete d;

    delete ui;
}

void MainWindow::setLanguage(const QString &language)
{
    d->localeName = language;
}

QString MainWindow::getLanguage() const
{
    return d->localeName;
}

void MainWindow::setAdmxPath(const QString &admxPath)
{
    if (d->options.policyBundle.trimmed().isEmpty())
    {
        d->options.policyBundle = admxPath;
    }
}

QString MainWindow::getAdmxPath() const
{
    return d->options.policyBundle;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    d->settings->saveSettings();

    QMainWindow::closeEvent(event);
}

void MainWindow::loadPolicyModel(ISnapIn *snapIn, const QString &locale)
{
    Q_UNUSED(locale);

    d->model = std::unique_ptr<QAbstractItemModel>(snapIn->getRootNode());

    d->itemNameSortModel = std::make_unique<QSortFilterProxyModel>();
    d->itemNameSortModel->setSourceModel(d->model.get());
    d->itemNameSortModel->setSortLocaleAware(true);
    d->itemNameSortModel->setSortRole(Qt::DisplayRole);
    d->itemNameSortModel->sort(0);

    d->itemRoleSortModel = std::make_unique<QSortFilterProxyModel>();
    d->itemRoleSortModel->setSourceModel(d->itemNameSortModel.get());
    d->itemRoleSortModel->setSortRole(model::bundle::PolicyRoles::ITEM_TYPE);
    d->itemRoleSortModel->sort(0);

    d->searchModel = std::make_unique<QSortFilterProxyModel>();
    d->searchModel->setSourceModel(d->itemRoleSortModel.get());
    d->searchModel->setFilterRole(Qt::DisplayRole);
    d->searchModel->setFilterFixedString("");
    d->searchModel->setRecursiveFilteringEnabled(true);

    ui->treeView->setModel(d->searchModel.get());

    d->contentWidget->setModel(d->searchModel.get());

    d->contentWidget->setSelectionModel(ui->treeView->selectionModel());

    ui->treeView->expand(d->searchModel->index(0, 0));
    ui->treeView->setColumnHidden(1, true);

    d->contentWidget->modelItemSelected(d->searchModel->index(0, 0));
}

void MainWindow::onDirectoryOpen()
{
    std::unique_ptr<QFileDialog> fileDialog = std::make_unique<QFileDialog>(this);

    fileDialog->setDirectory(QDir::homePath());
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setSupportedSchemes(QStringList(QStringLiteral("file")));

    fileDialog->setLabelText(QFileDialog::Accept, tr("Open"));
    fileDialog->setLabelText(QFileDialog::FileName, tr("File name"));
    fileDialog->setLabelText(QFileDialog::LookIn, tr("Look in"));
    fileDialog->setLabelText(QFileDialog::Reject, tr("Cancel"));
    fileDialog->setLabelText(QFileDialog::FileType, tr("File type"));

    fileDialog->setNameFilter(QObject::tr("All files (*.*)"));
    fileDialog->setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                           | QFileDialog::DontUseNativeDialog);

    fileDialog->setWindowIcon(d->windowIcon);
    fileDialog->setWindowTitle(tr("Open Directory"));

    if (fileDialog->exec() == QDialog::Accepted)
    {
        d->options.policyBundle = fileDialog->selectedUrls().value(0).toLocalFile();
    }
}

void MainWindow::onRegistrySourceSave()
{
    for (auto snapIn : d->manager->getSnapIns())
    {
        snapIn->onDataSave();
    }

    ui->statusbar->showMessage(tr("Applied changes for policy: ") + d->itemName);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionManual_triggered()
{
    const QUrl manual_url = QUrl(
        "https://www.altlinux.org/"
        "%D0%93%D1%80%D1%83%D0%BF%D0%BF%D0%BE%D0%B2%D1%8B%D0%B5_%D0%BF%D0%BE%D0%BB%D0%B8%D1%82%D0%B8%D0%BA%D0%B8/GPUI");
    QDesktopServices::openUrl(manual_url);
}

void MainWindow::on_actionAbout_triggered()
{
    auto about = new AboutDialog(this);
    about->show();
}

void MainWindow::onLanguageChanged(QAction *action)
{
    for (const auto &translator : d->translators)
    {
        qApp->removeTranslator(translator.get());
    }
    d->translators.clear();

    QString language = action->data().toString();

    for (auto &snapIn : d->manager->getSnapIns())
    {
        snapIn->onRetranslateUI(language.toStdString());
    }

    std::unique_ptr<QTranslator> qtTranslator = std::make_unique<QTranslator>();
    bool loadResult                           = qtTranslator->load("gui_" + language + ".qm", ":/");
    QCoreApplication::installTranslator(qtTranslator.get());
    d->translators.push_back(std::move(qtTranslator));
    qWarning() << "Load language " << language << loadResult;

    QLocale locale(language);

    d->localeName = locale.name().replace("_", "-");

    d->contentWidget->onLanguageChanged();
    ui->retranslateUi(this);

    ui->treeView->selectionModel()->clearSelection();

    ui->searchLineEdit->clear();
}

void MainWindow::onIniFileOpen(const QString &path)
{
    qWarning() << "Path recieved: " << path;

    auto stringvalues = std::make_unique<std::string>();

    try
    {
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
        std::string pluginName("ini");

        auto reader  = std::make_unique<io::GenericReader>();
        auto iniFile = reader->load<io::IniFile, io::PolicyFileFormat<io::IniFile>>(*iss, pluginName);
        if (!iniFile)
        {
            qWarning() << "Unable to load registry file contents.";
            return;
        }

        auto sections = iniFile->getAllSections();

        if (d->options.policyName.isEmpty() && sections->find("General") != sections->end())
        {
            auto &generalValues = (*sections)["General"];

            auto displayName = generalValues.find("displayName");

            if (displayName != generalValues.end())
            {
                qWarning() << "display name " << displayName.value().c_str();

                setWindowTitle(QString::fromStdString("GPUI - " + displayName.value()));
            }
        }
    }
    catch (std::exception &e)
    {
        qWarning() << "Warning: Unable to read file: " << qPrintable(path) << " description: " << e.what();
    }
}

void MainWindow::createLanguageMenu()
{
    QActionGroup *langGroup = new QActionGroup(this);
    langGroup->setExclusive(true);

    connect(langGroup, &QActionGroup::triggered, this, &MainWindow::onLanguageChanged);

    QString defaultLocale = d->localeName.left(QLocale::system().name().lastIndexOf('_'));
    QDir dir(":/");
    QStringList fileNames = dir.entryList(QStringList("gui_*.qm"));

    QMenu *menu = new QMenu(this);
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

} // namespace gpui
