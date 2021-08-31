#include "smbfilebrowser.h"
#include "ui_smbfilebrowser.h"

#include <libnemofolderlistmodel/dirmodel.h>
#include <libnemofolderlistmodel/dirselection.h>

#include <QDir>
#include <QMetaType>
#include <QHeaderView>
#include <QDebug>
#include <QProgressBar>
#include <QMessageBox>
#include <QTimer>
#include <QLineEdit>
#include <QMouseEvent>
#include <QDialog>

void SmbFileBrowser::onRowClicked(QModelIndex index)
{
    if (index.isValid())
    {
        d->selection->select(index.row(), false, false);
    }
}

void SmbFileBrowser::onSetSort(int col, Qt::SortOrder sortOrder)
{
    if (col == DirModel::SortByName || col == DirModel::SortByDate)
    {
        DirModel::SortOrder order = (DirModel::SortOrder)sortOrder;
        d->model->setSortOrder(order);
    }
}

void SmbFileBrowser::onClipboardChanged()
{

}

void SmbFileBrowser::onProgress(int cur, int total, int percent)
{
    Q_UNUSED(total);

    QString p;
    ui->progressBar->setValue(percent);
    if (cur == 0 && percent == 0)
    {
        ui->progressBar->reset();
        ui->progressBar->show();
    }
    else if (percent == 100)
    {
        QTimer::singleShot(200, ui->progressBar, SLOT(hide()));
    }
}

void SmbFileBrowser::onError(QString title, QString message)
{
    QMessageBox::critical(this, title, message);
}

void SmbFileBrowser::onOpenItem(QModelIndex index)
{
    if (index.isValid())
    {
        QModelIndex idx = d->model->index(index.row(), 0);
        QString item = d->model->data(idx).toString();

        if (item.endsWith(".pol"))
        {
            onPolOpen(d->model->path() + QDir::separator() + item);
            this->accept();
            return;
        }

        d->model->openIndex(index.row());
    }
}


void SmbFileBrowser::onPathChanged(QString path)
{
    path = d->model->path();

    int index = ui->comboBoxPath->findText(path);
    if (index == -1)
    {
        ui->comboBoxPath->insertItem(0, path);
        ui->comboBoxPath->setCurrentIndex(0);
    }
    else
    {
        ui->comboBoxPath->setCurrentIndex(index);
    }
    this->setWindowTitle(path);
}


void SmbFileBrowser::onPathChoosedFromList(int row)
{
    d->model->setPath(ui->comboBoxPath->itemText(row));
}


void SmbFileBrowser::onPathComboEdited()
{
    d->model->openPath(ui->comboBoxPath->lineEdit()->text());
}


void SmbFileBrowser::onSelectionChanged(int itemsCounter)
{
    Q_UNUSED(itemsCounter);
}

void SmbFileBrowser::cdUp()
{
    d->model->cdUp();
}

void SmbFileBrowser::goBack()
{
    d->model->openPath(d->previousPath);
}

void SmbFileBrowser::onAwaitingResultsChanged()
{
    if (d->model->awaitingResults())
    {
        this->setCursor(Qt::BusyCursor);
    }
    else
    {
         this->setCursor(Qt::ArrowCursor);
    }
}


void SmbFileBrowser::onRowsInserted(QModelIndex, int, int)
{
   ui->tableViewFM->resizeColumnToContents(0);
}

