#ifndef GPUI_SMB_FILE_BROWSER_H
#define GPUI_SMB_FILE_BROWSER_H

#include <QDialog>
#include <QModelIndex>

#include <memory>

namespace Ui {
    class SmbFileBrowser;
}

class DirModel;
class DirSelection;

class QProgressBar;

class SmbFileBrowserPrivate
{
public:
    std::unique_ptr<DirModel> model;
    DirSelection *selection;
    QString previousPath;

    SmbFileBrowserPrivate();
};


class SmbFileBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit SmbFileBrowser(QWidget *parent = 0);
    ~SmbFileBrowser();

signals:
    void onPolOpen(const QString& path);

private:
    Ui::SmbFileBrowser          *ui;
    SmbFileBrowserPrivate       *d;

private slots:
    void onRowClicked(QModelIndex);
    void onOpenItem(QModelIndex index);
    void onSetSort(int col, Qt::SortOrder order);
    void onProgress(int, int,int);
    void onClipboardChanged();
    void onError(QString title, QString message);
    void onPathChanged(QString newPath);
    void onPathChoosedFromList(int);
    void onPathComboEdited();
    void onSelectionChanged(int);
    void onAwaitingResultsChanged();
    void onRowsInserted(QModelIndex, int,int);

    void cdUp();
    void goBack();

private:
    void do_connections();
};

#endif // GPUI_SMB_FILE_BROWSER_H
