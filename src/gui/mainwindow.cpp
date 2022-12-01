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

#include "../plugins/administrative_templates/bundle/itemtype.h"
#include "../plugins/administrative_templates/bundle/policybundle.h"
#include "../plugins/administrative_templates/bundle/policyroles.h"

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

#include "../ldap/ldapimpl.h"

#include <stack>

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

    std::unique_ptr<ldap::LDAPContract> ldapImpl = nullptr;

    MainWindowPrivate()
        : eventFilter(new TreeViewEventFilter())
        , ldapImpl(new ldap::LDAPImpl())
    {}

private:
    MainWindowPrivate(const MainWindowPrivate &) = delete;            // copy ctor
    MainWindowPrivate(MainWindowPrivate &&)      = delete;            // move ctor
    MainWindowPrivate &operator=(const MainWindowPrivate &) = delete; // copy assignment
    MainWindowPrivate &operator=(MainWindowPrivate &&) = delete;      // move assignment
};

QModelIndex findParent(QAbstractItemModel *model, const QModelIndex &parent, const QUuid &id)
{
    auto stack = std::make_unique<std::stack<QModelIndex>>();
    stack->push(parent);

    while (!stack->empty())
    {
        auto current = stack->top();
        stack->pop();

        auto currentId = current.data(Qt::UserRole + 12).value<QUuid>();
        if (currentId == id)
        {
            return current;
        }

        for (int row = 0; row < model->rowCount(current); ++row)
        {
            QModelIndex index = model->index(row, 0, current);
            auto indexId      = index.data(Qt::UserRole + 12).value<QUuid>();
            if (indexId == id)
            {
                return index;
            }

            if (model->hasChildren(index))
            {
                for (int childRow = 0; childRow < model->rowCount(index); ++childRow)
                {
                    QModelIndex childIndex = model->index(childRow, 0, index);
                    stack->push(childIndex);
                }
            }
        }
    }

    return QModelIndex();
}

void appendModel(QStandardItem *target, const QAbstractItemModel *model, const QModelIndex &parent)
{
    for (int rowIndex = 0; rowIndex < model->rowCount(parent); ++rowIndex)
    {
        QModelIndex index = model->index(rowIndex, 0, parent);

        auto parentId    = index.data(Qt::UserRole + 13).value<QUuid>();
        auto parentIndex = QModelIndex();
        auto currentId   = index.data(Qt::UserRole + 12).value<QUuid>();

        auto currentIndex    = findParent(target->model(), target->model()->index(0, 0).parent(), currentId);
        QStandardItem *child = nullptr;

        if (!currentIndex.isValid())
        {
            if (!currentId.isNull())
            {
                qWarning() << "Current id: " << currentId;
            }
            if (!parentId.isNull())
            {
                qWarning() << "Non null uuid" << index.data() << parentId;
                parentIndex = findParent(target->model(), target->model()->index(0, 0), parentId);
            }

            child = new QStandardItem();
            child->setData(index.data(Qt::DisplayRole), Qt::DisplayRole);
            child->setData(index.data(Qt::DecorationRole), Qt::DecorationRole);
            child->setData(index.data(model::bundle::ITEM_TYPE), model::bundle::ITEM_TYPE);
            child->setData(index.data(model::bundle::EXPLAIN_TEXT), model::bundle::EXPLAIN_TEXT);
            child->setData(index.data(model::bundle::SUPPORTED_ON), model::bundle::SUPPORTED_ON);
            child->setData(index.data(model::bundle::PRESENTATION), model::bundle::PRESENTATION);
            child->setData(index.data(model::bundle::POLICY), model::bundle::POLICY);
            child->setData(index.data(model::bundle::POLICY_TYPE), model::bundle::POLICY_TYPE);
            child->setData(index.data(model::bundle::POLICY_WIDGET), model::bundle::POLICY_WIDGET);
            child->setData(index.data(Qt::UserRole + 12), Qt::UserRole + 12);
            child->setData(index.data(Qt::UserRole + 13), Qt::UserRole + 13);

            if (parentIndex.isValid())
            {
                qWarning() << "Found valid index" << parentIndex.data();
                target->model()->itemFromIndex(parentIndex)->appendRow(child);
            }
            else
            {
                target->insertRow(rowIndex, child);
            }
        }
        else
        {
            child = target->model()->itemFromIndex(currentIndex);
        }

        if (model->hasChildren(index))
        {
            appendModel(child, model, index);
        }
    }
}

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

    d->ldapImpl->initialize();

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
    connect(ui->actionSaveRegistrySource, &QAction::triggered, this, &MainWindow::updateStatusBar);
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
        snapIn->onInitialize(this);
    }

    if (!d->options.path.isEmpty())
    {
        for (auto &snapIn : manager->getSnapIns())
        {
            snapIn->onDataLoad(d->options.path.toStdString(), d->localeName.toStdString());
        }
    }

    QString guid = isAnyGUID(d->options.path);

    qWarning() << "Guid: " << guid;

    if (guid != "")
    {
        d->options.policyName = " - " + d->ldapImpl.get()->getDisplayNameGPO(guid);
    }

    loadPolicyModel(manager);

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
    if (!d->options.policyBundle.trimmed().isEmpty())
    {
        d->options.policyBundle = admxPath;
        admxPathChanged(admxPath);
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

void MainWindow::loadPolicyModel(ISnapInManager *manager)
{
    auto concatenateRowsModel = std::make_unique<QStandardItemModel>();

    QStandardItem *rootItem = concatenateRowsModel->invisibleRootItem();
    rootItem->setData("Root Item", Qt::DisplayRole);

    QStandardItem *visibleRootItem = new QStandardItem();
    visibleRootItem->setData(QIcon::fromTheme("text-x-generic-template"), Qt::DecorationRole);
    visibleRootItem->setData(static_cast<uint>(model::bundle::ItemType::ITEM_TYPE_CATEGORY), model::bundle::ITEM_TYPE);
    visibleRootItem->setData(QObject::tr("Local group policies"), model::bundle::EXPLAIN_TEXT);
    visibleRootItem->setData(static_cast<uint>(model::admx::PolicyType::Both), model::bundle::POLICY_TYPE);

    if (d->options.path.startsWith("smb://"))
    {
        QRegExp domainRegexp("^(?:smb?:\\/\\/)?([^:\\/\\n?]+)");
        if (domainRegexp.indexIn(d->options.path) != -1)
        {
            visibleRootItem->setData('[' + domainRegexp.cap() + ']' + d->options.policyName, Qt::DisplayRole);
        }
        else
        {
            visibleRootItem->setData(QObject::tr("[Domain Group Policy]") + d->options.policyName, Qt::DisplayRole);
        }
    }
    else
    {
        visibleRootItem->setData(QObject::tr("[Local Group Policy]") + d->options.policyName, Qt::DisplayRole);
    }

    rootItem->appendRow(visibleRootItem);

    for (auto &snapIn : manager->getSnapIns())
    {
        if (snapIn->getRootNode())
        {
            QAbstractItemModel *snapInModel = snapIn->getRootNode();
            qWarning() << "Appending model from: " << snapIn->getDisplayName();
            appendModel(visibleRootItem, snapInModel, snapInModel->index(0, 0));
        }
    }

    d->model = std::move(concatenateRowsModel);

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
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
        setAdmxPath(fileDialog->selectedUrls().value(0).toLocalFile());
        loadPolicyModel(d->manager);
    }
}

void MainWindow::updateStatusBar()
{
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

    std::unique_ptr<QTranslator> qtTranslator = std::make_unique<QTranslator>();
    bool loadResult                           = qtTranslator->load("gui_" + language + ".qm", ":/");
    QCoreApplication::installTranslator(qtTranslator.get());
    d->translators.push_back(std::move(qtTranslator));
    qWarning() << "Load language " << language << loadResult;

    QLocale locale(language);

    d->localeName = locale.name().replace("_", "-");

    for (auto &snapIn : d->manager->getSnapIns())
    {
        snapIn->onRetranslateUI(d->localeName.toStdString());
        qWarning() << d->localeName;
    }

    d->contentWidget->onLanguageChanged();
    ui->retranslateUi(this);

    loadPolicyModel(d->manager);

    ui->treeView->selectionModel()->clearSelection();

    ui->searchLineEdit->clear();
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

QString MainWindow::isAnyGUID(QString &path)
{
    QRegExp lastPartOfPath("/\\{([^/]+)\\}/?$");
    QRegExp regExpGuid("^([a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12})$");

    qWarning() << lastPartOfPath.indexIn(path);

    if (lastPartOfPath.indexIn(path) != -1)
    {
        QStringList lastPart = lastPartOfPath.capturedTexts();

        QString preGuid = lastPart[lastPart.size() - 1];

        if (regExpGuid.indexIn(preGuid) != -1)
        {
            return preGuid;
        }
        else
        {
            return QString();
        }
    }
    else
    {
        return QString();
    }
}

} // namespace gpui
