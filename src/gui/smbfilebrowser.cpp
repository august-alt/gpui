#include "smbfilebrowser.h"

#include "ui_smbfilebrowser.h"

#include <libnemofolderlistmodel/dirmodel.h>
#include <libnemofolderlistmodel/dirselection.h>

#include <memory>

#include <QDir>
#include <QLineEdit>
#include <QDialog>
#include <QHeaderView>
#include <QTableView>

SmbFileBrowserPrivate::SmbFileBrowserPrivate()
    : model(new DirModel())
{}

SmbFileBrowser::SmbFileBrowser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SmbFileBrowser())
    , d(new SmbFileBrowserPrivate())
{
    ui->setupUi(this);

    ui->tableViewFM->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);

    DirModel::registerMetaTypes();

    ui->tableViewFM->setModel(d->model.get());

    d->model->setEnabledExternalFSWatcher(true);

    d->selection = d->model->selectionObject();

    ui->tableViewFM->setSelectionMode(QAbstractItemView::NoSelection);

    do_connections();

    d->model->goHome();
}

SmbFileBrowser::~SmbFileBrowser()
{
    delete ui;
    delete d;
}

void SmbFileBrowser::do_connections()
{
    connect(ui->tableViewFM, &QTableView::clicked, this, &SmbFileBrowser::onRowClicked);

    connect(ui->tableViewFM, &QTableView::doubleClicked, this, &SmbFileBrowser::onOpenItem);

    connect(d->model.get(), &DirModel::pathChanged, this, &SmbFileBrowser::onPathChanged);

    connect(ui->toolButtonUp, &QToolButton::clicked, this, &SmbFileBrowser::cdUp);
    connect(ui->toolButtonBack, &QToolButton::clicked, this, &SmbFileBrowser::goBack);

    connect(ui->comboBoxPath, SIGNAL(activated(int)), this, SLOT(onPathChoosedFromList(int)));

    connect(ui->comboBoxPath->lineEdit(), &QLineEdit::returnPressed, this, &SmbFileBrowser::onPathComboEdited);

    connect(ui->tableViewFM->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &SmbFileBrowser::onSetSort);

    connect(d->model.get(), &DirModel::progress, this, &SmbFileBrowser::onProgress);

    connect(d->model.get(), &DirModel::clipboardChanged, this, &SmbFileBrowser::onClipboardChanged);

    connect(d->model.get(), &DirModel::error, this, &SmbFileBrowser::onError);

    connect(d->model.get(), &DirModel::awaitingResultsChanged, this, &SmbFileBrowser::onAwaitingResultsChanged);

    connect(d->model.get(), &DirModel::rowsInserted, this, &SmbFileBrowser::onRowsInserted);

    connect(d->selection, &DirSelection::selectionChanged, this, &SmbFileBrowser::onSelectionChanged);
}
